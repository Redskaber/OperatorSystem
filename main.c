#include "main.h"

void test_allocators() {
//    test_initSystemResource_withValidInputs_initializesSystemResourceCorrectly();
//    test_initSystemResource_withZeroInputs_initializesSystemResourceCorrectly();
//    test_initSystemResource_withNegativeInputs_initializesSystemResourceCorrectly();
}

void test_Banker() {
//    test_initBanker_withValidInputs_initializesBankerCorrectly();
//    test_initBanker_withEmptyResourceArray_initializesBankerCorrectly();
//    test_initBanker_withNegativeNumberRows_initializesBankerCorrectly();
}

void testBankerSecurity() {
//    test_checkResourceSecurity_withSafeSequence_returnsTrue();
//    test_checkResourceSecurity_withUnsafeSequence_returnsFalse();
}

int main() {
    test_allocators();
    test_Banker();
    testBankerSecurity();
    test_allocator();

    return 0;
}
