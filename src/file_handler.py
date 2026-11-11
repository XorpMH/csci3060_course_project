def readFile(file_path):
    """
    Reads an entire file line by line and returns a list of lines.
    """
    try:
        with open(file_path, "r", encoding="utf-8") as file:
            lines = []
            for line in file:
                lines.append(line.rstrip("\n"))
            return lines
    except FileNotFoundError:
        return []


def writeFile(file_path, list_lines):
    """
    Writes a list of lines to a file.
    Existing file contents are replaced.
    """
    with open(file_path, "w", encoding="utf-8") as file:
        for line in list_lines:
            file.write(str(line) + "\n")


def appendFile(file_path, line):
    """
    Adds one line to the bottom of a file.
    """
    with open(file_path, "a", encoding="utf-8") as file:
        file.write(str(line) + "\n")


def removeFirstFile(file_path, line):
    """
    Compares the given line with each line in the file
    and removes the first matching line.
    """
    lines = readFile(file_path)
    new_lines = []
    removed = False
    for current_line in lines:
        if current_line == line and not removed:
            removed = True
            continue
        new_lines.append(current_line)
    writeFile(file_path, new_lines)
    return removed

    