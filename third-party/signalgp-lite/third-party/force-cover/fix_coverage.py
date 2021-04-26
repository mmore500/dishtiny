#!/usr/bin/env python

# Author: Emily Dolson
# Copyright: 2018
# License: MIT

"""
fix_coverage.py is a script for modifying C++ coverage reports to
mark uninstantiated templates as executable regions missing coverage.

Usage: ./fix_coverage.py [coverage_report_file]

Assumes that coverage file has the format:
[source_line_number] | [execution_count]| [code]

./force_coverage must have been run on the source code before it was
compiled in order for this to work.
"""

import sys
import re


def de_comment_line(line):
    """
    Returns a version of the string @line with all
    C++ comments removed
    """
    line = re.sub(r"/\*.*\*/", "", line)  # Remove /* comments
    line = re.sub(r"//.*", "", line)  # Remove // comments
    return line


def validate_line(line, second_bar):
    """
    Returns true if this line contains code that should be executre.
    Arguments: line - a string containing the full text of this line
               second_bar - an int indicating the index in the string
                            at which the second "|" character occurs,
                            denoting the start of the code portion.
    """

    if second_bar == -1:  # This isn't a valid code coverage line
        return False

    line = line[second_bar+1:].strip()  # Get the code part of the line
    line = de_comment_line(line)  # Remove all comments

    if line == "":              # Don't report empty lines as uncovered
        return False
    if line == "{":             # A single brace can't be executed
        return False
    if line == "}":
        return False

    # Have another rule for skipping lines? Add it here!

    return True


def is_first_line(line):
    """
    Returns true if line is the first line of a new file
    """
    return line.split("|")[0].strip() == "1"


def is_functionlike_macro_definition(line):
    """
    Returns true if this line contains a function-like macro definition
    (e.g. #define TEST(a) a)
    """

    if re.findall(r".*#define\s\S*\(.*\)\s", line):
        # Function-like macros can't have spaces between name and
        # parentheses. This should be the only way this line
        # could be one
        return True

    return False


def is_macro_continuation(line):
    """
    Takes a line and returns a boolean indicating whether or not
    a macro defined on this line would be continued on the next line
    (i.e. is the last non-comment character a "\" ?)
    """
    line = de_comment_line(line)
    if line.endswith("\\"):
        return True
    return False


def opens_multiline_comment(line):
    """
    Return true if the string @param line contains a
    /* without a following */, meaning it opens a multiline
    comment but does not close it.
    """

    line = de_comment_line(line)
    return len(re.findall(r"/\*", line)) > 0


def closes_multiline_comment(line):
    """
    Return true if the string @param line contains a
    */ without a preceeding /*, meaning it closes an
    already-open multiline comment.
    """

    line = de_comment_line(line)
    return len(re.findall(r"\*/", line)) > 0


def cover_line(line):
    """
    This function takes a string containing a line that should
    potentially have an execution count and returns a version
    of that line that does have an execution count if deemed
    appropriate by the rules in validate_line().

    Basically, if there is currently no number where there should
    be an execution count (indicating this line did not make
    it into the compiled binary), a zero is placed there to
    indicate that this line was executed 0 times. Test coverage
    viewers will interpret this to mean that the line could
    potentially have been executed.
    """
    first_bar = line.find("|")
    second_bar = line.find("|", first_bar+1)

    if validate_line(line, second_bar) and \
            line[second_bar-1].strip() == "":
        # If this line could have been executed but wasn't (no
        # number between first and second bars), put a zero
        # before the second bar, indicating that it was
        # executed zero times. Test coverage viewers will interpret
        # this as meaning the line should have been covered
        # but wasn't.
        return "".join([line[:second_bar-1],
                        "0", line[second_bar:]])

    # There's already an execution count - this
    # template must have been instantiated
    return line


def main():

    # TODO: At some point we'll probably want options, at which point we should
    # use an actual argument parser
    if len(sys.argv) < 2:
        print("Usage: python fix_coverage.py [name_of_coverage_report_file]")
        exit(1)

    lines = []  # Accumulate list of lines to glue back together at the end
    with open(sys.argv[1]) as infile:
        force_cover_active = 0  # Counter of open nested templates
        macro_active = False  # Are we in a function-like macros definition?
        multi_line_comment_active = False  # Are we in a multi-line comment?

        for line in infile:

            # ~~~~~~~~~~~~~~~~~~ Book-keeping section ~~~~~~~~~~~~~~~~~~~~~~
            # (adjusts force_cover_active count)

            line = line.rstrip()  # Remove trailing whitespace

            if is_first_line(line):
                # Zero out regions at beginning of new file just
                # in case stuff got screwed up
                force_cover_active = 0

            # We don't know how many template function definitions start on
            # this line, but we know each will be preceeded by this string
            force_cover_active += line.count("_FORCE_COVER_START_")

            # Special case where region of forced coverage starts at
            # the end of this line (so we don't count the part before
            # the body as executable)
            if line.endswith("/*_FORCE_COVER_START_*/") and \
                    force_cover_active == line.count("_FORCE_COVER_START_"):
                lines.append(line + "\n")
                continue

            # ~~~~~~~~~~~~~ Multi-line comment section ~~~~~~~~~~~~~~~~~~~~~~
            # If we're in a multi-line comment nothing else matters

            if opens_multiline_comment(line):
                multi_line_comment_active = True
            if closes_multiline_comment(line):
                multi_line_comment_active = False
            if multi_line_comment_active:
                lines.append(line + "\n")
                continue

            # ~~~~~~~~~~~~ Function-like macro section ~~~~~~~~~~~~~~~~~~~~~~

            # Function-like macros defined in the code can be tested just
            # like any other code, but they don't show up in the AST the
            # way other stuff does (because the pre-processor handles them).
            # So, we're going to manually check for them here so we can
            # report when they haven't been covered by tests.
            # TODO: Make this optional
            if is_functionlike_macro_definition(line):
                macro_active = True

            if macro_active:  # Add coverage data if we're in a macro
                line = cover_line(line)
                if not is_macro_continuation(line):
                    # If this macro doesn't have a continuation character,
                    # then this is the last line of it
                    macro_active = False

            # ~~~~~~~~~~~~~~~ Template coverage section ~~~~~~~~~~~~~~~~~~~~~~
            # (where we do the actual covering of potentially
            # uninstantiated templates)

            if not force_cover_active:  # Don't need to change line because
                lines.append(line + "\n")      # we aren't in a template definition
                continue

            # In template. Might need to do stuff. cover_line() will figure
            # it out
            lines.append(cover_line(line) + "\n")

            # ~~~~~~~~~~~~~~~~ Closing book-keeping section ~~~~~~~~~~~~~~~~~
            # (adjusts force_cover_active count)

            # Closing force_cover comments happens at the end of the line
            force_cover_active -= line.count("_FORCE_COVER_END_")

    # Rewrite file with modified execution counts
    with open(sys.argv[1], "w") as infile:
        infile.writelines(lines)

if __name__ == "__main__":
    main()
