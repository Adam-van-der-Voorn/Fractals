import os
import re       

def rec(dir):
    cmakelists = os.path.join(dir, "CMakeLists.txt")

    # get all .cpp & .h files in dir
    dir_contents = os.listdir(dir)
    nested_dirs = [item for item in dir_contents if not os.path.isfile(os.path.join(dir, item))]
    source_files = [item for item in dir_contents if os.path.splitext(item)[-1].lower() == '.cpp']

    # convert to indented, newline seperated string of filenames
    sources = "".join(["    ${CMAKE_CURRENT_LIST_DIR}/%s\n" % f.replace('\\', '/') for f in source_files])
    

    # replace the old target sources with the new
    with open(cmakelists, 'r') as file:
        text = file.read()
    text = re.sub(r"target_sources\(\${PROJECT_NAME} PRIVATE.*\)",
            "".join(["target_sources(${PROJECT_NAME} PRIVATE", '\n', sources, ')']), 
            text, 1, re.DOTALL)

    with open(cmakelists, "w") as file:
        file.write(text)

    for d in nested_dirs:
        rec(os.path.join(dir, d))
   
# current directory
dir = os.path.dirname(os.path.realpath(__file__))
rec(os.path.join(dir, 'src'))



