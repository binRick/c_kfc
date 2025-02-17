#include "kfc-test/kfc-test.h"
#include "kfc-utils/kfc-utils.h"

void __attribute__((destructor)) __kfc_test_destructor(){
}


TEST t_kfc_utils_module(void){
  int res = 0;

  //kfc_utils_module_test();

  ASSERT_EQ(res, 0);
  PASS();
}


SUITE(s_kfc){
  RUN_TEST(t_kfc_utils_module);
  PASS();
}

GREATEST_MAIN_DEFS();


int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  RUN_SUITE(s_kfc);
  GREATEST_MAIN_END();
  return(0);
}
