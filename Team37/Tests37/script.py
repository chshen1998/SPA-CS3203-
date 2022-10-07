import os
import sys
import shutil
from pathlib import Path

AUTOTESTER_PATH = Path("../Code37/cmake-build-debug/src/autotester/autotester")

if sys.platform == "win32":
    AUTOTESTER_PATH = Path("../Code37/out/build/x64-Debug/src/autotester/autotester")

OUTPUT_XML_PATH = Path("./out.xml")

milestones = [milestone.name for milestone in Path(".").iterdir() if milestone.is_dir()]

shell_cmds = []

OUTPUT_FOLDER = Path("./TestOutputs/")

if "TestOutputs" in milestones:
    shutil.rmtree(OUTPUT_FOLDER)

OUTPUT_FOLDER.mkdir()

for milestone in milestones:
    if milestone == "TestOutputs" or milestone == "TestCases-Progress":
        continue

    Path(f"./TestOutputs/{milestone}").mkdir()

    print(f"Scanning {milestone}")
    testcase_folders = [folder.name for folder in Path('./').joinpath(milestone).iterdir()]

    for testcase_folder in testcase_folders:
        Path(f"./TestOutputs/{milestone}/{testcase_folder}").mkdir()

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
                if 'Missing:' in f.read() or 'Additional:   ' in f.read():
                    print("Testcase Failing: ", f"{milestone}/{testcase_folder}/{testcase}")

print("Completed scanning all files")
