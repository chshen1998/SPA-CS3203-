import os
import sys
import shutil
from pathlib import Path


AUTOTESTER_PATH = Path("../Code37\build\src\autotester\Release\autotester")

print(f"--- OS: {sys.platform}, Mode: {buildType} ---\n")

OUTPUT_XML_PATH = Path("./out.xml")

milestones = [milestone.name for milestone in Path(".").iterdir() if milestone.is_dir()]

shell_cmds = []

OUTPUT_FOLDER = Path("./TestOutputs/")

if "TestOutputs" in milestones:
    shutil.rmtree(OUTPUT_FOLDER)

OUTPUT_FOLDER.mkdir()

exception_milestone_folders = ["Sample SIMPLE Code", "TestOutputs", "TestCases-Progress"]

incomplete_testcases = []
failed_testcases = []

for milestone in milestones:
    if milestone in exception_milestone_folders:
        continue

    Path(f"./TestOutputs/{milestone}").mkdir()

    print(f"-- Scanning {milestone} -- ")
    testcase_folders = [folder.name for folder in Path('./').joinpath(milestone).iterdir()]

    for testcase_folder in testcase_folders:
        if testcase_folder == ".DS_Store":
            continue
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

        passed_testcases = []

        for testcase in testcases:
            testcase_source_path = Path(f"./{milestone}/{testcase_folder}/{source_file}")
            testcase_path = Path(f"./{milestone}/{testcase_folder}/{testcase}")

            output_path = Path(f"./TestOutputs/{milestone}/{testcase_folder}/{testcase}")
            shell_cmd = f"{AUTOTESTER_PATH} {testcase_source_path} {testcase_path} {OUTPUT_XML_PATH} > {output_path} "
            os.system(shell_cmd)

            with open(output_path) as f:
                filetxt = f.read()
                if 'Missing:' in filetxt or 'Additional:   ' in filetxt or "TIMEOUT" in filetxt:
                    failed_testcases.append(f"Failed\t{testcase_folder}/{testcase}")

                elif 'End of evaluating Query File.' not in filetxt:
                    incomplete_testcases.append(f"Not Completed\t{testcase_folder}/{testcase}")

                else:
                    passed_testcases.append(f"Passed\t{testcase_folder}/{testcase}")

        for passed_testcase in passed_testcases:
            print(passed_testcase)

    print("")

for incomplete_testcase in incomplete_testcases:
    print(incomplete_testcase)
for failed_testcase in failed_testcases:
    print(failed_testcase)
print("Completed scanning all files")
