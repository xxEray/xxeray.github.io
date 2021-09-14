@echo off

set "msg=%1"
if "msg" == "" set "msg=update content" 

git checkout source

git add -A
git commit -m %msg%
git push origin source

cmd /k "hexo d & exit"

cmd /k "hexo clean & exit"
rmdir /s/q ".deploy_git"