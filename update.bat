@echo off

cd %~dp0

set msg=%1
if [%msg%] == [] set msg="update content"

git checkout source > nul

echo Github: Pushing source...
git add -A > nul
git commit -m %msg% > nul
git push origin source -f > nul

echo Github: Cleaning...
cmd /k "hexo clean & exit" > nul
rmdir /s/q ".deploy_git" > nul