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

  if not os.path.exists(inputAbsolute):
    os.makedirs(inputAbsolute)

  if not os.path.exists(outputAbsolute):
    os.makedirs(outputAbsolute)

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
doFile("..\\template\\Template.h", "..\\src\\%s\\%s.h" % (folder, classname), classname, namespace)
doFile("..\\template\\Template.cpp", "..\\src\\%s\\%s.cpp" % (folder, classname), classname, namespace)
doFile("..\\template\\TestTemplate.cpp", "..\\test\\test\\%s\\%sTest.cpp" % (folder, classname), classname, namespace)
print("Done!")

