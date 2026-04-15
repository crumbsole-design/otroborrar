import pytest

from src.transformations import parse_vector


def test_parse_vector_valid():
    assert parse_vector("1 2") == (1.0, 2.0)
    assert parse_vector("  -3.5\t4.25 ") == (-3.5, 4.25)


def test_parse_vector_invalid():
    with pytest.raises(ValueError):
        parse_vector("1")
    with pytest.raises(ValueError):
        parse_vector("a b")
    with pytest.raises(ValueError):
        parse_vector("1 2 3")
