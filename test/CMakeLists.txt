CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(RULES_TEST)

INCLUDE(OCLintConfig)

INCLUDE_DIRECTORIES(
    ${OCLINT_SOURCE_DIR}/include
    ${OCLINT_METRICS_SOURCE_DIR}/include
    ${OCLINT_RULES_SOURCE_DIR}/include
    ${OCLINT_RULES_SOURCE}
    )

LINK_DIRECTORIES(
    ${OCLINT_BUILD_DIR}/lib
    ${OCLINT_METRICS_BUILD_DIR}/lib
    )

ADD_EXECUTABLE(test_rule
    main.cpp
    )
TARGET_LINK_LIBRARIES(test_rule
    ${CLANG_LIBRARIES}
    ${REQ_LLVM_LIBRARIES}
    OCLintCore
    OCLintMetric
    OCLintAbstractRule
    OCLintRuleSet
    OCLintHelper
    OCLintUtil
    )
