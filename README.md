# Arduino Coursework

## StudentID - F422019

## Project Overview
This repositary contains my coursework for WSA024. I am programming a temperature sensor to read the temperature in its surrounding environment once a second for three minutes and then using this data to decide what mode the sensor should be in and how frequently it should measure the temperature in the room. To programme this sensor I am using an arduino board. The system needs to be low power as this helps minimize energy consumption as well as not using up lots of memory to store the various temperatures.


## Task 1.1
The reason for this issue is that the automatic settings within google colab is that it only has the permissions to access public repositary's. To fix this the student needs to open the Github settings and go into developer settings and then personal access tokens to allow you to upload notebooks from Google Colab. To confirm that this has worked the student needs to try and save their notebook into their private repositary again and then open Github to see if it has appeared, it should appear with the students commit message displayed.

## Task 1.2
This task involved making a Github action workflow that automatically verifies tasks 1.3 and 1.4. This is helpful as it gets rid of the need for manual checks and catches errors early. It helps determine if the test-branch is deleted successfully and if the merge conflict is resolved by looking for specific commit messages.

## task 1.3
after carrying out the task this message was displayed:
error: cannot delete branch 'test-branch' used by worktree at '/workspaces/F422019_WSA024_Arduino_Coursework'
To fix this the user must switch to a different branch before trying to delete the test-branch. This error occurs because Github needs at least one branch to operate so it therefore prevents you from deleting the branch you are on. to change branch type in 'git checout main'. Then you should try and delete the branch using 'git branch -d test-branch' and it should work.

## Task 1.4
This is the message displayed after attempting to merge the feature-branch into the main:
Auto-merging project-notes.md
CONFLICT (add/add): Merge conflict in project-notes.md
Automatic merge failed; fix conflicts and then commit the result.
This message is displayed because of a conflict between both branches on the same lines. Git needs this to be fixed manually as it cannot decide which one is correct on its own. This is to prevetn accidental data loss. To fix it i opened project-notes.md and edited the file to resolve the conflict. I then typed in "git add project-notes.md" followed by "git commit -m "Resolved merge conflict in project-notes.md"". This creates a commit with the -m letting you leave a message which tells you what the commit is about. Then to finish it off i typed in "git push origin main". To prevent merge conflicts users must carry out frequent pulls as this allows users to stay up to date with other peoples work and prevents conflicts. To prevetn conflict before merging changes the best solution would be to pull the latest changes from the remote branch first and then merge. This helps to see if there are any conflicts early.
