@echo off
for /f "tokens=*" %%a in ('git diff-tree --no-commit-id --name-only -r HEAD') do (
    echo %%~xa | findstr /i ".cpp .h .hpp .c" >nul && (
        echo Formatting %%a
        clang-format -i --style=file %%a
    )
)