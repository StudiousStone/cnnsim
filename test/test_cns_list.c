#include "test_cnnsim.h"
#include "../src/cns_util.h"
#include "../src/cns_list.h"

static int *data;
static size_t data_len;
static cns_list *list;

static void setup(void)
{
	int i;
	list = NULL;
	data_len = 5;
	data = cns_alloc(sizeof(int) * data_len);
	for (i = 0; i < 5; i++) {
		data[i] = i;
		list = cns_list_append(list, &data[i]);
	}
}

static void teardown(void)
{
	cns_free(data);
	cns_list_free(list);
}

START_TEST(test_list_append_nth)
{
	cns_list *l;

	l = cns_list_append(NULL, &data[0]);
	l = cns_list_append(l, &data[1]);
	ck_assert_int_eq(*(int *)cns_list_nth_data(l, 0), 0);
	ck_assert_int_eq(*(int *)cns_list_nth_data(l, 1), 1);
	cns_list_free(l);
}
END_TEST

START_TEST(test_list_remove)
{
	int num, i;

	list = cns_list_remove(list, &data[0]);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 0), 1);

	list = cns_list_insert_nth(list, &data[0], 0);
	list = cns_list_remove(list, &data[4]);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 3), 3);
	ck_assert_ptr_eq(cns_list_nth_data(list, 4), NULL);

	list = cns_list_insert_nth(list, &data[4], 4);
	num = -1;
	list = cns_list_remove(list, &num);
	for (i = 0; i < data_len; i++)
		ck_assert_int_eq(*(int *)cns_list_nth_data(list, i), data[i]);

	num = 5;
	list = cns_list_remove(list, &num);
	for (i = 0; i < data_len; i++)
		ck_assert_int_eq(*(int *)cns_list_nth_data(list, i), data[i]);
}
END_TEST

START_TEST(test_list_remove_insert_nth)
{
	int i;

	list = cns_list_remove_nth(list, 2);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 2), 3);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 1), 1);

	list = cns_list_insert_nth(list, &data[2], 2);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 2), 2);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 3), 3);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 1), 1);

	list = cns_list_remove_nth(list, 0);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 0), 1);

	list = cns_list_insert_nth(list, &data[0], 0);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 0), 0);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 1), 1);

	list = cns_list_remove_nth(list, 4);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 3), 3);
	ck_assert_ptr_eq(cns_list_nth_data(list, 4), NULL);

	list = cns_list_insert_nth(list, &data[4], 4);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 4), 4);
	ck_assert_int_eq(*(int *)cns_list_nth_data(list, 3), 3);

	list = cns_list_remove_nth(list, -1);
	for (i = 0; i < data_len; i++)
		ck_assert_int_eq(*(int *)cns_list_nth_data(list, i), data[i]);

	list = cns_list_remove_nth(list, 6);
	for (i = 0; i < data_len; i++)
		ck_assert_int_eq(*(int *)cns_list_nth_data(list, i), data[i]);
}
END_TEST

START_TEST(test_list_find)
{
	cns_list *l;
	int n1 = 6;
	int n2 = -1;

	l = cns_list_find(list, &data[3]);
	ck_assert_int_eq(*(int *)l->data, data[3]);

	l = cns_list_find(list, &n1);
	ck_assert_ptr_eq(l, NULL);

	l = cns_list_find(list, &n2);
	ck_assert_ptr_eq(l, NULL);
}
END_TEST

static int cmp(void *a, void *b)
{
	return *(int *)a - *(int *)b;
}

START_TEST(test_list_find_custom)
{
	cns_list *l;
	int n1 = 6;
	int n2 = -1;

	l = cns_list_find_custom(list, &data[3], &cmp);
	ck_assert_int_eq(*(int *)l->data, data[3]);

	l = cns_list_find_custom(list, &n1, &cmp);
	ck_assert_ptr_eq(l, NULL);

	l = cns_list_find_custom(list, &n2, &cmp);
	ck_assert_ptr_eq(l, NULL);
}
END_TEST

START_TEST(test_list_position)
{
	int pos;
	cns_list *l;

	pos = cns_list_position(list, list->next);
	ck_assert_int_eq(pos, 1);

	l = (cns_list *)cns_alloc(sizeof(cns_list));
	pos = cns_list_position(list, l);
	ck_assert_int_eq(pos, -1);

	pos = cns_list_position(list, NULL);
	ck_assert_int_eq(pos, -1);
}
END_TEST

START_TEST(test_list_index)
{
	int pos;
	int n;

	pos = cns_list_index(list, list->next->data);
	ck_assert_int_eq(pos, 1);

	n = 6;
	pos = cns_list_index(list, &n);
	ck_assert_int_eq(pos, -1);

	pos = cns_list_index(list, NULL);
	ck_assert_int_eq(pos, -1);
}
END_TEST

Suite *make_list_suite(void)
{
	Suite *s;
	s = suite_create("list");

	TCase *tc_list;
	tc_list = tcase_create("tc_list");
	tcase_add_checked_fixture(tc_list, setup, teardown);
	tcase_add_test(tc_list, test_list_append_nth);
	tcase_add_test(tc_list, test_list_remove);
	tcase_add_test(tc_list, test_list_remove_insert_nth);
	tcase_add_test(tc_list, test_list_find);
	tcase_add_test(tc_list, test_list_find_custom);
	tcase_add_test(tc_list, test_list_position);
	tcase_add_test(tc_list, test_list_index);
	suite_add_tcase(s, tc_list);

	return s;
}
