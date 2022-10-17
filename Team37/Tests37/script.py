import os
import sys
import shutil
from pathlib import Path

AUTOTESTER_PATH = Path("../Code37/cmake-build-release/src/autotester/autotester")

buildType = "Release"

if sys.platform == "win32":
    AUTOTESTER_PATH = Path("../Code37/out/build/x64-{}/src/autotester/autotester".format(buildType))

print(f"--- OS: {sys.platform}, Mode: {buildType} ---\n")

OUTPUT_XML_PATH = Path("./out.xml")

milestones = [milestone.name for milestone in Path(".").iterdir() if milestone.is_dir()]

shell_cmds = []

OUTPUT_FOLDER = Path("./TestOutputs/")

if "TestOutputs" in milestones:
    shutil.rmtree(OUTPUT_FOLDER)

OUTPUT_FOLDER.mkdir()

exception_milestone_folders = ["Sample SIMPLE Code", "TestOutputs", "TestCases-Progress"]

for milestone in milestones:
    if milestone in exception_milestone_folders:
        continue

    Path(f"./TestOutputs/{milestone}").mkdir()

    print(f"-- Scanning {milestone} -- ")
    testcase_folders = [folder.name for folder in Path('./').joinpath(milestone).iterdir()]

    for testcase_folder in testcase_folders:
        Path(f"./TestOutputs/{milestone}/{testcase_folder}").mkdir()

        if testcase_folder == ".DS_Store":
            continue
        directory_path = Path("./{}/{}".format(milestone, testcase_folder))
        test_files = [f.name for f in directory_path.iterdir()]

        testcases = []
        source_file = ""

        for test_file in test_files:

            if "source" in test_file:
                source_file = test_file
            elif "queries" in test_file:
                testcases.append(test_file)
        
        for testcase in testcases:
            testcase_source_path = Path(f"./{milestone}/{testcase_folder}/{source_file}")
            testcase_path = Path(f"./{milestone}/{testcase_folder}/{testcase}")

            output_path = Path(f"./TestOutputs/{milestone}/{testcase_folder}/{testcase}")
            shell_cmd = f"{AUTOTESTER_PATH} {testcase_source_path} {testcase_path} {OUTPUT_XML_PATH} > {output_path} "
            os.system(shell_cmd)

            with open(output_path) as f:
                filetxt = f.read()
                if 'Missing:' in filetxt or 'Additional:   ' in filetxt:
                    print(f"Failed\t{testcase_folder}/{testcase}")

                elif 'End of evaluating Query File.' not in filetxt:
                    print(f"Not Completed\t{testcase_folder}/{testcase}")

                else:
                    print(f"Passed\t{testcase_folder}/{testcase}")

    print("")


print("Completed scanning all files")
