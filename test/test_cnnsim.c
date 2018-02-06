#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "test_cnnsim.h"

int main(int argc, char **argv)
{
	int number_failed;
	int status;
	SRunner *sr;

	sr = srunner_create(make_block_suite());
	srunner_add_suite(sr, make_tensor_suite());
	srunner_add_suite(sr, make_list_suite());
	srunner_add_suite(sr, make_queue_suite());
	srunner_add_suite(sr, make_graph_suite());

	srunner_set_xml (sr, "result/check_output.xml");
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	status = system("sed -i 's,http://check.sourceforge.net/xml/check_unittest.xslt,check_unittest.xslt,g' result/check_output.xml");
	if (status < 0)
		fprintf(stderr, "system() error\n");

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
