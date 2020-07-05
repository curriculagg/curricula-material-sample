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


@grader.setup.check(sanity=True)
def check_decimal_header(context: Context, resources: dict):
    header_path = resources["decimal"] = context.problem_path.joinpath("decimal.hpp")
    return check_file_exists(header_path)


@grader.setup.build(sanity=True, passing={"check_decimal_header"})
def build_decimal_tests(context: Context, resources: dict):
    result, resources["decimal_tests"] = build_gpp_executable(
        root.joinpath("tests.cpp"),
        destination_path=context.problem_path.joinpath("tests"),
        gpp_options=(f"-I{context.problem_path}", f"-I{include}", *GPP_OPTIONS),
        timeout=60)
    return result


TESTS = (
    ("initialize_empty", 1),
    ("initialize_string_zero", 2),
    ("initialize_string_integer", 2),
    ("initialize_string_integer_negative", 2),
    ("initialize_string_decimal", 2),
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


for test_name, test_weight in TESTS:
    grader.test.correctness(
        name=test_name,
        passing={"build_decimal_tests"},
        weight=test_weight
    )(ExecutableOutputCodeTest(
        executable_name="decimal_tests",
        args=(test_name,)))


@grader.teardown.cleanup(passing={"build_decimal_tests"})
def cleanup_decimal_tests(decimal_tests: ExecutableFile):
    delete_file(decimal_tests.path)
