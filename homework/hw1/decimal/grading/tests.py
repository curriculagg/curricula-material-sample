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


TEST_NAMES = (
    "initialize_empty",
    "initialize_string_zero",
    "initialize_string_integer",
    "initialize_string_integer_negative",
    "initialize_string_decimal",
    "initialize_string_decimal_short",
    "initialize_string_decimal_long",
)


class ExecutableOutputCodeTest(ExecutableCodeMixin, CompareExitCodeOutputTest):
    """Run executable and check output."""

    expected_code = 0


for test_name in TEST_NAMES:
    grader.test.correctness(name=test_name, passing={"build_decimal_tests"})(ExecutableOutputCodeTest(
        executable_name="decimal_tests",
        args=(test_name,)))


@grader.teardown.cleanup(passing={"build_decimal_tests"})
def cleanup_decimal_tests(decimal_tests: ExecutableFile):
    delete_file(decimal_tests.path)
