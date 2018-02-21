# agrismarter_project
## how to combine two repositories:
https://www.smashingmagazine.com/2014/05/moving-git-repository-new-server/  
  git fetch origin  
  git branch -a  
  git checkout -b develop origin/develop  
  git remote add new-origin git@github.com:manakor/manascope.git  
  git push --all new-origin  
  git push --tags new-origin //optional  
  git remote rm origin  
  git remote rename new-origin origin  
