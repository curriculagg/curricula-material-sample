from curricula.grade.shortcuts import *
from curricula.grade.setup.check.common import check_file_exists
from curricula.grade.setup.build.common import build_gpp_executable
from curricula.grade.test.correctness.common import ExecutableMixin, CompareBytesOutputTest
from curricula.library.files import delete_file

from pathlib import Path
from typing import Iterable, Tuple

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

    def __init__(self, lines: Iterable[Tuple[bytes, bytes]]):
        equations = b"".join(line[0] + b"\n" for line in lines)
        expected = [line[1] for line in lines]
        super().__init__(stdin=equations, test_out_lines=expected)

    executable_name = "arithmetic"

    @staticmethod
    def out_transform(line: bytes) -> bytes:
        return line.strip()

    @staticmethod
    def load(string: str):
        test_name = None
        test_lines = []
        for line in string.strip().splitlines():
            if len(line.strip()) == 0:
                continue
            if not line[0].isspace():
                if test_name is not None:
                    grader.test.correctness(name=test_name)(ArithmeticTest(test_lines))
                    test_lines.clear()
                test_name = line.strip()
            else:
                equation, expected = line.split("=")
                test_lines.append((equation.encode().strip(), expected.encode().strip()))
        grader.test.correctness(name=test_name)(ArithmeticTest(test_lines))


ArithmeticTest.load("""
zero
    0 = 0

zero_twice
    0 = 0
    0 = 0

value
    123 = 123
    3.333 = 3.333

value_negative
    -123 = -123
    -3.333 = -3.333

value_negative_multiple
    --5 = 5
    ---5 = -5

value_paren
    (10) = 10
    -(-100) = 100

value_paren_nested
    ((100)) = 100
    ((((-1)))) = -1

addition_basic
    1 + 1 = 2
    10 + 20 = 30

addition_chained
    1 + 1 + 1 = 3
    10 + 20 + 30 = 60

addition_paren
    (1 + (2 + (3 + (4 + 5)))) = 15

addition_negative_no_precedence
    -1 + 1 = 0
    -10 + 20 = 10
    -20 + 10 = -10

addition_negative_precedence
    1 + -1 = 0
    10 + -20 = -10

addition_decimal_any
    -1.25 + 1.25 = 0
    0.10 + 0.20 = 0.3
    0.10 + 0.20 + 0.30 = 0.6
    20.20 + -10.10 = 10.1

addition_chained_many
    1 + -1 + 1 + -1 + 1 + -1 = 0
    11.1 + -22.2 + 33.3 - 44.4 + 55.5 - 66.6 + 77.7 = 44.4

addition_negative_many
    -1 + --1 = 0
    5 + --6 + -1 = 10

subtraction_basic
    1 - 1 = 0
    10 - 20 = -10

subtraction_chained
    3 - 1 - 1 - 1 = 0
    20 - 10 - 5 = 5

subtraction_negative_no_precedence
    -1 - 1 = -2
    -20 - 10 - 10 = -40

subtraction_negative_precedence
    1 - -1 = 2
    20 - -10 = 30
    -10 - -20 - -30 = 40

subtraction_negative_many
    -1 - --1 = -2
    5 - --6 - -1 = 0

multiplication_any
    1 * 1 = 1
    2 * 2 = 4
    -7 * 10 = -70

multiplication_decimal_any
    0.333 * 3 = 0.999
    1.5 * 1.5 = 2.25

division_any
    10 / 2 = 5
    1 / 1 = 1
    5 / -10 = -0.5

division_decimal_any
    3.5 / 0.5 = 7
    100 / 6.25 = 16
    0.3 / 0.4 = 0.75

division_zero
    10 / 0 = divide by zero
    0 / 0 = divide by zero

mixed_addition_subtraction
    10 + 5 - 5 = 10
    -33.3 - 33.3 + -33.3 = -99.9

mixed_multiplication_division
    30 / 5 * 6 = 36
    12.5 * 8 / 0.5 = 200
    0 / 10 = 0

mixed_addition_multiplication
    10 + -5 * -5 = 35
    12 * 5 + 12 = 72
    -12 * -12 + 12 * -12 = 0
    0.5 * 12 + 2 * -3 = 0

mixed_addition_multiplication_paren
    (10 + -5) * -5 = -25
    12 * (5 + 12) = 204
    -12 * (-12 + 12) * -12 = 0
    0.5 * (12 + 2) * -3 = -21

mixed_any
    35 * (-123 + 37) / 100 * (50 + -20) - 10 = -913
    (0.5 * 0.5 * 0.5 + (3 + 7) / 10) * 10 / 100 = 0.1125
    -5 * (-5 + 12.5) / 10 / 10 = -0.375
""")


@grader.teardown.cleanup(passing={"build_arithmetic"})
def cleanup_arithmetic(arithmetic: ExecutableFile):
    delete_file(arithmetic.path)
