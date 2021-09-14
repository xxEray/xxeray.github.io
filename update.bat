@echo off

cd %~dp0

set "msg=%1"
if "msg" == "" set "msg=update content" 

git checkout source > nul 2> nul

echo Github: Pushing source...
git add -A > nul 2> nul
git commit -m %msg% > nul 2> nul
git push origin source > nul 2> nul

echo Github: Pushing master...
cmd /k "hexo d & exit" > nul 2> nul

echo Github: Cleaning...
cmd /k "hexo clean & exit" > nul 2> nul
rmdir /s/q ".deploy_git" > nul 2> nul