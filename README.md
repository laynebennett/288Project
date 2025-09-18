# 2880-Final-Project

##Short Description
There is a taped-out home square where the robot will go to restock food (a variable to keep track of the truck’s stock will be refreshed). The CyBot will drive down the road until it sees a medium sized object (a road sign in this case) which will indicate for the robot to turn. Once the robot travels to the drop off point, it will park against the curb (short sized objects). The CyBot will scan to see if there is a customer in line on the other side of the curb. If there is a customer, we subtract the food stock variable by 1. If we run out of food and there are still people, we drive back to the home square to refresh our stock and drive back to the drop off zone until there are no more customers left. 

While driving on the road, occasionally there will be obstacles like walking pedestrians (small width objects) or a collapsed tree (large width object) that the cybot will need to either navigate around or wait out before continuing to either the home or to the drop off zone. 

##Implementation
The CyBot is an iRobot Roomba 600 controlled via a Tiva TM4C123GH6PM and controlled via wifi input using PuTTY. On the CyBot utilizes an IR sensor mounted on a servo, and various other sensors mentioned in the iRobot Create 2 OI. The GUI is written in Python and run in VScode. 


## Git mini-tutorial
### Getting the repo locally
First, go to a terminal of choice (assuming that you have git installed on your computer). This works with the typical windows terminal, but git bash is my preferred terminal

Use the cd command to navigate through the file structure in order to find where you want to place the repo. For example, I placed the repo in my X: drive in a folder called
CPRE2880. So the command I would enter is "cd X:/CPRE2880".

Once here, you need to clone the repo from the cloud. From the repo, there will be a green button that says "code" and has a dropdown arrow on it. If you click on it, there
will be an option to clone with HTTPS (this should be chosen by default, but if it isn't just click on the HTTPS tab in the code dropdown menu). There should be an https link
shown. Copy the link, and go back into your terminal.

From the terminal, write "git clone [https link here] [folder name]" where https link is what you copied from github and the folder name is whatever you want to call the folder
locally on your computer. For example, I named my folder "final-project".

After entering this command, a popup asking you to sign into github may appear. Just sign in and you should be good.

### Creating a branch
When working on the repo, make sure that you're working on your own separate feature branch so that way we don't accidentally run into merge conflicts
or step on each others toes.

To do this, make sure that you use the cd command to enter the folder for the repo (ie "cd [folder name]". Once you're in, if you're using git bash, there should be a blue
piece of text that tells you what branch you're in. At this point, it should say "(main)").

Now to actually create the branch. You can do this one of two ways:

First, go to github and next to the the button that shows the name of the current branch, there will be another button that says "X branches". Click this, and click the
green "new branch" button. This will pull up a text box to name the branch and a dropdown to select the source branch. You should be safe to assume the source branch will
always be main, unless you are making a sub branch of another branch. Leave it as main (unless otherwise you need a sub branch), and name your branch (try to name it based
on whatever feature you are working on. i.e. if you're working on the UART, name it something to do with UART). Go back to the terminal and run "git fetch origin" just to
be sure that the branch is updated locally on your machine, and then use "git checkout [branch name]" to switch your working directory to your new branch.

Second, if you want to do it through terminal, write "git checkout -b [branch name]" using the same naming convention as outlined in the first option. This will create and
change your terminal directory to your new branch. Then, run "git push --set-upstream origin [branch name]" to push the branch to the cloud repo.

If you want to switch back and forth between branches, just use "git checkout [branch name]". Please make sure that you are on the correct branch when making changes.

### Saving changes locally
Lets say you edit uart.c and want to save the changes. First, go to your terminal and (optionally) run "git status" to see what changes you made. This will show a list of files
in either red or green. Red means that those files have been changed and not saved and green means that they have been added to the next commit. From here, you can either run
"git add [filename]" to add the individual files you want to save OR "git add --all" to add everything that you changed at once.

Once you've added your files, write "git commit -m "[Write a brief summary of your changes. Doesn't need to be more than a sentence as long as it gets the point across]"". This
will save the files into a local commit for you to push to the cloud later.

### Pushing your changes to the cloud
Once you're done making changes, you can run "git push" to push any saved local commits to the cloud repo. This will allow anyone to view your changes from the github website, and
download your changes to their machine to work on them locally.

## That's it
If you have any other questions, just message me on discord. -Conner
