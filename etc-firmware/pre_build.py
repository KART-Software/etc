import subprocess

Import("env")

# compiledb 実行中はこの pre script を即終了
if "compiledb" in COMMAND_LINE_TARGETS:
    Return()

project_dir = env.subst("$PROJECT_DIR")
python_exe = env.subst("$PYTHONEXE")
pioenv = env.subst("$PIOENV")

subprocess.run(
    [
        python_exe,
        "-m",
        "platformio",
        "run",
        "-d",
        project_dir,
        "-e",
        pioenv,
        "-t",
        "compiledb",
    ],
    check=False,
)
