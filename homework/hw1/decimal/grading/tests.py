from curricula.grade.shortcuts import *
from curricula.grade.setup.check.common import check_file_exists
from curricula.grade.setup.build.common import build_gpp_executable
from curricula.grade.test.correctness.common import ExecutableCodeMixin, CompareExitCodeOutputTest
from curricula.grade import include
from curricula.library.files import delete_file

from pathlib import Path

root = Path(__file__).absolute().parent
grader = Grader()

GPP_OPTIONS = ("-std=c++14", "-Wall")


@grader.setup.check(tags={"sanity"})
def check_decimal_header(submission: Submission, resources: dict):
    """decimal.hpp present in decimal/"""

    header_path = resources["decimal"] = submission.problem_path.joinpath("decimal.hpp")
    return check_file_exists(header_path)


@grader.setup.build(tags={"sanity"}, passing={"check_decimal_header"})
def build_decimal_tests(submission: Submission, resources: dict):
    """our decimal test harness builds with decimal.hpp"""

    result, resources["decimal_tests"] = build_gpp_executable(
        root.joinpath("tests.cpp"),
        destination_path=submission.problem_path.joinpath("tests"),
        gpp_options=(f"-I{submission.problem_path}", f"-I{include}", *GPP_OPTIONS),
        timeout=60)
    return result


TESTS = (
    ("initialize_empty", 1, "empty decimal initialization works", {"sanity"}),
    ("initialize_string_zero", 2, "decimal initializes \"0\" correctly", {"sanity"}),
    ("initialize_string_integer", 2, "decimal initializes with integer strings correctly", {"sanity"}),
    ("initialize_string_integer_negative", 2),
    ("initialize_string_decimal", 2, "decimal initializes with decimal strings correctly", {"sanity"}),
    ("initialize_string_decimal_short", 2),
    ("initialize_string_decimal_long", 2),
    ("initialize_string_decimal_round", 2),
    ("initialize_decimal", 1),
    ("operator_equal", 2),
    ("operator_equal_different_radix", 1),
    ("operator_inequal", 2),
    ("operator_greater", 2),
    ("operator_less", 2),
    ("operator_greater_equal", 1),
    ("operator_less_equal", 1),
    ("operator_plus", 2),
    ("operator_minus", 2),
    ("operator_times", 3),
    ("operator_times_overflow", 1),
    ("operator_times_rounding", 1),
    ("operator_divide", 3),
    ("operator_divide_rounding", 1),
)


class ExecutableOutputCodeTest(ExecutableCodeMixin, CompareExitCodeOutputTest):
    """Run executable and check output."""

    expected_code = 0


for test_name, test_weight, *rest in TESTS:
    grader.test.correctness(
        name=test_name,
        passing={"build_decimal_tests"},
        description=rest[0] if len(rest) >= 1 else None,
        tags=rest[1] if len(rest) >= 2 else set(),
        weight=test_weight
    )(ExecutableOutputCodeTest(
        executable_name="decimal_tests",
        args=(test_name,)))


@grader.teardown.cleanup(passing={"build_decimal_tests"})
def cleanup_decimal_tests(decimal_tests: ExecutableFile):
    delete_file(decimal_tests.path)
