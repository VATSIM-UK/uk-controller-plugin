import os

# Get input
classname = input("Enter name of new class: ")
namespace = input("Enter namespace of new class: ")
folder = namespace.lower()

print("New class to be created: %s in namespace %s" % (classname, namespace))
print("Header file will be created at src/%s/%s.h" % (folder, classname))
print("Implementation file will be created at src/%s/%s.cpp" % (folder, classname))
print("Test file will be created at test/test/%s/%sTest.cpp" % (folder, classname))

response = input("Y to confirm: ")
if (response.lower() != "y"):
  print("Not confirmed, stopping");

def doFile(inputFile, outputFile, className, namespace):

  # Get file paths
  inputAbsolute = os.path.join(os.path.abspath(os.path.dirname(__file__)), inputFile)
  outputAbsolute = os.path.join(os.path.abspath(os.path.dirname(__file__)), outputFile)

  if not os.path.exists(os.path.dirname(inputAbsolute)):
    os.makedirs(os.path.dirname(inputAbsolute))

  if not os.path.exists(os.path.dirname(outputAbsolute)):
    os.makedirs(os.path.dirname(outputAbsolute))

  ## Open template, replace placeholders
  fileHandle = open(inputAbsolute, "r+")
  templateContents = fileHandle.read()
  fileHandle.close()

  templateContents = templateContents.replace("##NAMESPACE##", namespace)
  templateContents = templateContents.replace("##CLASSNAME##", className)
  templateContents = templateContents.replace("##FOLDERNAME##", namespace.lower())

  # Write to output
  outputHandle = open(outputAbsolute, "w")
  outputHandle.write(templateContents)
  outputHandle.close()

# Do each template
sep = os.path.sep
doFile(f"..{sep}template{sep}Template.h", f"..{sep}src{sep}{folder}{sep}{classname}.h", classname, namespace)
doFile(f"..{sep}template{sep}Template.cpp", f"..{sep}src{sep}{folder}{sep}{classname}.cpp", classname, namespace)
doFile(f"..{sep}template{sep}TestTemplate.cpp", f"..{sep}test{sep}test{sep}{folder}{sep}{classname}Test.cpp", classname, namespace)
print("Done!")

