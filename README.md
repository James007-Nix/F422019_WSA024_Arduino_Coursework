# Arduino Coursework

## StudentID - F422019

## Project Overview
This repositary contains my coursework for WSA024. I am programming a temperature sensor to read the temperature in its surrounding environment once a second for three minutes and then using this data to decide what mode the sensor should be in and how frequently it should measure the temperature in the room. To programme this sensor I am using an arduino board. The system needs to be low power as this helps minimize energy consumption as well as not using up lots of memory to store the various temperatures.


## Task 1.1
The reason for this issue is that the automatic settings within google colab is that it only has the permissions to access public repositary's. To fix this the student needs to open the Github settings and go into developer settings and then personal access tokens to allow you to upload notebooks from Google Colab. To confirm that this has worked the student needs to try and save their notebook into their private repositary again and then open Github to see if it has appeared, it should appear with the students commit message displayed.

## Task 1.2
This task involved making a Github action workflow that automatically verifies tasks 1.3 and 1.4. This is helpful as it gets rid of the need for manual checks and catches errors early. It helps determine if the test-branch is deleted successfully and if the merge conflict is resolved by looking for specific commit messages.
