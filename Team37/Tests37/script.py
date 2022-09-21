import os
import shutil

AUTOTESTER_PATH = "../Code37/cmake-build-debug/src/autotester/autotester" 
OUTPUT_XML_PATH= "./out.xml"

milestones = [milestone for milestone in os.listdir('.') if os.path.isdir(milestone)]

shell_cmds = []

OUTPUT_FOLDER = "./TestOutputs/"

shutil.rmtree(OUTPUT_FOLDER)
os.mkdir(OUTPUT_FOLDER)

for milestone in milestones:
    if milestone == "TestOutputs":
        continue
    os.mkdir(f"./TestOutputs/{milestone}")

    print(f"Scanning {milestone}")
    milestone_folders = os.listdir('./'+milestone)
    testcase_folders = [folder for folder in milestone_folders]\
        
    for testcase_folder in testcase_folders:
        os.mkdir(f"./TestOutputs/{milestone}/{testcase_folder}")

        directory_path = "./{}/{}".format(milestone,testcase_folder)
        test_files = os.listdir(directory_path)
        
        testcases = []
        source_file = ""
        for test_file in test_files:
            
            if "source" in test_file:
                source_file = test_file
            elif "queries" in test_file:
                testcases.append(test_file)
                                
        for testcase in testcases:
            testcase_source_path = f"./{milestone}/{testcase_folder}/{source_file}"
            testcase_path = f"./{milestone}/{testcase_folder}/{testcase}"
            
            output_path = f"./TestOutputs/{milestone}/{testcase_folder}/{testcase}"
            shell_cmd= f"{AUTOTESTER_PATH} {testcase_source_path} {testcase_path} {OUTPUT_XML_PATH} > {output_path} "
            os.system(shell_cmd)
            
            with open(output_path) as f:
                if 'Missing' in f.read() or 'Additional' in f.read():
                    print("Testcase Failing: ",f"{milestone}/{testcase_folder}/{testcase}")
                    
print("Completed scanning all files")


