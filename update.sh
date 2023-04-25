#!/bin/bash

mypath=$(dirname $0)
cd ${mypath}

if [[ $? == 0 ]]; then
	 set msg="update content"
else
	msg=%1
fi

git checkout source > nul

echo Github: Pushing source...
git add -A > nul
git commit -m %msg% > nul
git push origin source -f > nul

echo Github: Pushing master...
hexo d > nul

echo Github: Cleaning...
hexo clean > nul
rmdir /s/q ".deploy_git" > nul

rm nul
