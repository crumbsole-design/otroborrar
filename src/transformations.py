from typing import Tuple, List
import math


def parse_vector(s: str) -> Tuple[float, float]:
    """Parse a vector from a string containing two real numbers.

    Raises ValueError on invalid input.
    """
    if not isinstance(s, str):
        raise ValueError("Input must be a string")
    parts = s.strip().split()
    if len(parts) != 2:
        raise ValueError("Vector input must contain exactly two numbers")
    try:
        x = float(parts[0])
        y = float(parts[1])
    except ValueError as exc:
        raise ValueError("Vector components must be real numbers") from exc
    return (x, y)


def parse_transformations(s: str) -> List[Tuple[str, float]]:
    """Parse a transformation sequence string into a list of (cmd, value).

    Commands are: 'ESCALA' <float>, 'ROT_DEG' <float>, sequence ends with ';'.
    """
    if not isinstance(s, str):
        raise ValueError("Transformations must be provided as a string")
    if not s.strip():
        raise ValueError("Empty transformations string")
    if not s.strip().endswith(";"):
        raise ValueError("Transformations sequence must end with ';'")
    body = s.strip()
    # remove trailing semicolon
    body = body[:-1].strip()
    if not body:
        return []
    tokens = body.split()
    i = 0
    result: List[Tuple[str, float]] = []
    while i < len(tokens):
        cmd = tokens[i]
        if cmd not in ("ESCALA", "ROT_DEG"):
            raise ValueError(f"Unknown transformation '{cmd}'")
        if i + 1 >= len(tokens):
            raise ValueError(f"Missing value for {cmd}")
        val_str = tokens[i + 1]
        try:
            val = float(val_str)
        except ValueError as exc:
            raise ValueError(f"Invalid numeric value for {cmd}: {val_str}") from exc
        result.append((cmd, val))
        i += 2
    return result


def apply_transformations(v: Tuple[float, float], transforms: List[Tuple[str, float]]) -> List[Tuple[float, float]]:
    """Apply transformations sequentially, returning intermediate vectors after each step."""
    if not (isinstance(v, tuple) and len(v) == 2):
        raise ValueError("Vector must be a tuple of two numbers")
    x, y = float(v[0]), float(v[1])
    results: List[Tuple[float, float]] = []
    for cmd, val in transforms:
        if cmd == "ESCALA":
            factor = float(val)
            x, y = x * factor, y * factor
        elif cmd == "ROT_DEG":
            ang_deg = float(val)
            ang = math.radians(ang_deg)
            cos_a = math.cos(ang)
            sin_a = math.sin(ang)
            nx = cos_a * x - sin_a * y
            ny = sin_a * x + cos_a * y
            x, y = nx, ny
        else:
            raise ValueError(f"Unknown transformation '{cmd}'")
        results.append((x, y))
    return results


def format_vector(v: Tuple[float, float]) -> str:
    return f"({v[0]:.3f}, {v[1]:.3f})"


def process(vector: Tuple[float, float], transforms_text: str) -> List[str]:
    """End-to-end processing: returns list of output lines (strings)."""
    if not (isinstance(vector, tuple) and len(vector) == 2):
        raise ValueError("Vector must be a tuple of two numbers")
    # initial line
    lines: List[str] = []
    lines.append(f"v = {format_vector((float(vector[0]), float(vector[1])))}")
    # parse transformations (may raise ValueError)
    transforms = parse_transformations(transforms_text)
    if not transforms:
        return lines
    results = apply_transformations((float(vector[0]), float(vector[1])), transforms)
    for (cmd, val), res in zip(transforms, results):
        if cmd == "ESCALA":
            lines.append(f"Escala a = {float(val):.3f}, v = {format_vector(res)}")
        elif cmd == "ROT_DEG":
            rad = math.radians(float(val))
            lines.append(f"Rotacion alfa = {rad:.3f} rad, v = {format_vector(res)}")
        else:
            raise ValueError(f"Unknown transformation '{cmd}'")
    return lines
