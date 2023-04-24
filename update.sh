#!/bin/bash

mypath=$(dirname $0)
cd ${mypath}

if [[ $? == 0 ]]; then
	 set msg="update content"
else
	msg=%1
fi

git checkout source > /dev/nul

echo Github: Pushing source...
git add -A > /dev/nul
git commit -m %msg% > /dev/nul
git push origin source -f > /dev/nul

echo Github: Pushing master...
hexo d > /dev/nul

echo Github: Cleaning...
hexo clean > /dev/nul
rmdir /s/q ".deploy_git" > /dev/nul