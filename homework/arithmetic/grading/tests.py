from curricula_grade.shortcuts import *

from curricula_grade.setup.common import check_file_exists
from curricula_grade_cpp.setup.common.gpp import GppObjectSetup
from curricula_grade.test.correctness.common import ProcessCompareStreamTest
from curricula.library.files import delete_file

from pathlib import Path
from typing import Iterable, Tuple, Iterator, List, Set

root = Path(__file__).absolute().parent
grader = Grader()

GPP_OPTIONS = ("-std=c++14", "-Wall")


@grader.register(tags={"sanity"})
def check_arithmetic_header(submission: Submission, resources: dict) -> SetupResult:
    """arithmetic.hpp present in arithmetic/"""

    arithmetic_header_path = resources["arithmetic_header_path"] = submission.problem_path.joinpath("arithmetic.hpp")
    return check_file_exists(arithmetic_header_path)


@grader.register(tags={"sanity"})
def check_arithmetic_source(submission: Submission, resources: dict) -> SetupResult:
    """arithmetic.cpp present in arithmetic/"""

    arithmetic_source_path = resources["arithmetic_source_path"] = submission.problem_path.joinpath("arithmetic.cpp")
    return check_file_exists(arithmetic_source_path)


@grader.register(tags={"sanity"})
def check_main_source(submission: Submission, resources: dict) -> SetupResult:
    """main.cpp present in arithmetic/"""

    main_source_path = resources["main_source_path"] = submission.problem_path.joinpath("main.cpp")
    return check_file_exists(main_source_path)


@grader.register(tags={"sanity"})
def check_decimal_header(submission: Submission, resources: dict) -> SetupResult:
    """decimal.hpp present in decimal."""

    decimal_problem = next(filter(lambda p: p.short == "decimal", grader.problem.assignment.problems))
    path = resources["decimal_include_path"] = submission.assignment_path.joinpath(decimal_problem.relative_path)
    return check_file_exists(path.joinpath("decimal.hpp"))


@grader.register(
    name="build_arithmetic",
    tags={"sanity"},
    passing={"check_arithmetic_header", "check_arithmetic_source", "check_main_source", "check_decimal_header"})
class BuildArithmetic(GppObjectSetup):
    """Build the arithmetic binary."""

    timeout = 60
    executable_name = "arithmetic"

    def get_source_paths(self, arithmetic_source_path: Path, main_source_path: Path) -> Iterable[Path]:
        return arithmetic_source_path, main_source_path

    def get_destination_path(self, submission: Submission) -> Path:
        return submission.problem_path.joinpath("arithmetic")

    def get_gpp_options(self, submission: Submission, decimal_include_path: Path) -> Iterable[str]:
        return f"-I{submission.problem_path}", f"-I{decimal_include_path}", *GPP_OPTIONS


class ArithmeticTest(ProcessCompareStreamTest):
    """Check that expressions evaluate correctly"""

    def __init__(self, lines: Iterable[Tuple[bytes, bytes]]):
        equations = b"".join(line[0] + b"\n" for line in lines)
        expected = [line[1] for line in lines]
        super().__init__(stdin=equations, test_out_lines=expected)

    executable_name = "arithmetic"

    @staticmethod
    def out_transform(line: bytes) -> bytes:
        return line.strip()

    @classmethod
    def load(cls, string: str):
        for test_name, test_description, test_tags, test_lines in cls.parse(string):
            grader.register[CorrectnessResult](
                name=test_name,
                tags=test_tags,
                description=test_description,
                passing={"build_arithmetic"}
            )(ArithmeticTest(test_lines))

    @classmethod
    def parse(cls, string: str) -> Iterator[Tuple[str, str, Set[str], List[Tuple[bytes, bytes]]]]:
        test_name = None
        test_description = None
        test_tags = set()
        test_lines = []
        for line in string.strip().splitlines():
            if len(line.strip()) == 0:
                continue
            if not line[0].isspace():
                if test_name is not None:
                    yield test_name, test_description, test_tags, test_lines
                    test_description = None
                    test_lines = []
                line_items = line.strip().split()
                test_name = line_items[0]
                test_tags = set(line_items[1:])
            elif line.strip().startswith("#"):
                test_description = line.strip()[1:].strip()
            else:
                equation, expected = line.split("=")
                test_lines.append((equation.encode().strip(), expected.encode().strip()))
        yield test_name, test_description, test_tags, test_lines


ArithmeticTest.load("""
zero sanity
    # Zero equals zero
    0 = 0.0000

zero_twice
    0 = 0.0000
    0 = 0.0000

value sanity
    # Values are stored correctly
    123 = 123.0000
    3.333 = 3.3330

value_negative
    -123 = -123.0000
    -3.333 = -3.3330

value_negative_multiple
    --5 = 5.0000
    ---5 = -5.0000

value_paren
    (10) = 10.0000
    -(-100) = 100.0000

value_paren_nested
    ((100)) = 100.0000
    ((((-1)))) = -1.0000

addition_basic sanity
    # Simple addition behaves as expected
    1 + 1 = 2.0000
    10 + 20 = 30.0000

addition_chained
    1 + 1 + 1 = 3.0000
    10 + 20 + 30 = 60.0000

addition_paren
    (1 + (2 + (3 + (4 + 5)))) = 15.0000

addition_negative_no_precedence
    -1 + 1 = 0.0000
    -10 + 20 = 10.0000
    -20 + 10 = -10.0000

addition_negative_precedence
    1 + -1 = 0.0000
    10 + -20 = -10.0000

addition_decimal_any
    -1.25 + 1.25 = 0.0000
    0.10 + 0.20 = 0.3000
    0.10 + 0.20 + 0.30 = 0.6000
    20.20 + -10.10 = 10.1000

addition_chained_many
    1 + -1 + 1 + -1 + 1 + -1 = 0.0000
    11.1 + -22.2 + 33.3 - 44.4 + 55.5 - 66.6 + 77.7 = 44.4000

addition_negative_many
    -1 + --1 = 0.0000
    5 + --6 + -1 = 10.0000

subtraction_basic
    1 - 1 = 0.0000
    10 - 20 = -10.0000

subtraction_chained
    3 - 1 - 1 - 1 = 0.0000
    20 - 10 - 5 = 5.0000

subtraction_negative_no_precedence
    -1 - 1 = -2.0000
    -20 - 10 - 10 = -40.0000

subtraction_negative_precedence
    1 - -1 = 2.0000
    20 - -10 = 30.0000
    -10 - -20 - -30 = 40.0000

subtraction_negative_many
    -1 - --1 = -2.0000
    5 - --6 - -1 = 0.0000

multiplication_any
    1 * 1 = 1.0000
    2 * 2 = 4.0000
    -7 * 10 = -70.0000

multiplication_decimal_any
    0.333 * 3 = 0.9990
    1.5 * 1.5 = 2.2500

division_any
    10 / 2 = 5.0000
    1 / 1 = 1.0000
    5 / -10 = -0.5000

division_decimal_any
    3.5 / 0.5 = 7.0000
    100 / 6.25 = 16.0000
    0.3 / 0.4 = 0.7500

division_zero
    10 / 0 = divide by zero
    0 / 0 = divide by zero

mixed_addition_subtraction
    10 + 5 - 5 = 10.0000
    -33.3 - 33.3 + -33.3 = -99.9000

mixed_multiplication_division
    30 / 5 * 6 = 36.0000
    12.5 * 8 / 0.5 = 200.0000
    0 / 10 = 0.0000

mixed_addition_multiplication
    10 + -5 * -5 = 35.0000
    12 * 5 + 12 = 72.0000
    -12 * -12 + 12 * -12 = 0.0000
    0.5 * 12 + 2 * -3 = 0.0000

mixed_addition_multiplication_paren
    (10 + -5) * -5 = -25.0000
    12 * (5 + 12) = 204.0000
    -12 * (-12 + 12) * -12 = 0.0000
    0.5 * (12 + 2) * -3 = -21.0000

mixed_any
    35 * (-123 + 37) / 100 * (50 + -20) - 10 = -913.0000
    (0.5 * 0.5 * 0.5 + (3 + 7) / 10) * 10 / 100 = 0.1125
    -5 * (-5 + 12.5) / 10 / 10 = -0.3750
""")


@grader.register[CleanupResult](passing={"build_arithmetic"})
def cleanup_arithmetic(arithmetic: ExecutableFile):
    delete_file(arithmetic.path)
