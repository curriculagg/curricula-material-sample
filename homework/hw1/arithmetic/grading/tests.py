from curricula.grade.shortcuts import *
from curricula.grade.setup.check.common import check_file_exists
from curricula.grade.setup.build.common import build_gpp_executable
from curricula.grade.test.correctness.common import ExecutableMixin, CompareBytesOutputTest
from curricula.library.files import delete_file

from pathlib import Path
from typing import Iterable

root = Path(__file__).absolute().parent
grader = Grader()

GPP_OPTIONS = ("-std=c++14", "-Wall")


@grader.setup.check(sanity=True)
def check_arithmetic_header(submission: Submission, resources: dict):
    arithmetic_header_path = resources["arithmetic_header_path"] = submission.problem_path.joinpath("arithmetic.hpp")
    return check_file_exists(arithmetic_header_path)


@grader.setup.check(sanity=True)
def check_arithmetic_source(submission: Submission, resources: dict):
    arithmetic_source_path = resources["arithmetic_source_path"] = submission.problem_path.joinpath("arithmetic.cpp")
    return check_file_exists(arithmetic_source_path)


@grader.setup.check(sanity=True)
def check_main_source(submission: Submission, resources: dict):
    main_source_path = resources["main_source_path"] = submission.problem_path.joinpath("main.cpp")
    return check_file_exists(main_source_path)


@grader.setup.build(sanity=True, passing={"check_arithmetic_header", "check_arithmetic_source", "check_main_source"})
def build_arithmetic(arithmetic_source_path: Path, main_source_path: Path, submission: Submission, resources: dict):
    result, resources["arithmetic"] = build_gpp_executable(
        arithmetic_source_path,
        main_source_path,
        destination_path=submission.problem_path.joinpath("arithmetic"),
        gpp_options=(f"-I{submission.problem_path}", *GPP_OPTIONS),
        timeout=60)
    return result


class ArithmeticTest(ExecutableMixin, CompareBytesOutputTest):
    """Run executable and check output."""

    def __init__(self, equations: Iterable[bytes], expected: Iterable[bytes]):
        super().__init__(stdin=b"\n".join(equations), test_out_lines=expected)

    executable_name = "arithmetic"

    @staticmethod
    def out_transform(line: bytes) -> bytes:
        return line.strip()


grader.test.correctness(name="zero")(ArithmeticTest(
    equations=(b"0", b"0"),
    expected=(b"0", b"0")))


@grader.teardown.cleanup(passing={"build_arithmetic"})
def cleanup_arithmetic(arithmetic: ExecutableFile):
    delete_file(arithmetic.path)
