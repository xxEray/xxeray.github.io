#!/bin/bash

mypath=$(dirname $0)
cd ${mypath}

if [[ $# == 0 ]]; then
	msg="update content"
else
	msg=$1
fi

git checkout source > /dev/null

echo Github: Pushing source...
git add -A > /dev/null
git commit -m "${msg}" > /dev/null
git push origin source -f > /dev/null

echo Github: Pushing master...
hexo d > /dev/null

echo Github: Cleaning...
hexo clean > /dev/null
rm -rf .deploy_git > /dev/null
