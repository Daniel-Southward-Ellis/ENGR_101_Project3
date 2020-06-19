<span id="anchor"></span>Install Instructions For Linux

1.  Download the project from this repo by clicking the clone or
    download button in the top right hand corner and selecting Download
    ZIP
2.  Find the downloaded file and Unzip it, this will vary based on your
    linux distro but in general you can do this either by double
    clicking the file and using an archive manager or by opening a
    terminal in the same folder as the ZIP file and running **unzip
    \<filename\>** where \<filename\> is replaced with the name of the
    file.
3.  Next download the Linux version of SFML from [*this
    link*](https://www.sfml-dev.org/download/sfml/2.5.1/), if you used
    an archive manager in the last step you can simply use it again the
    same way. If you used the unzip command in the last step you should
    be able to unzip it by opening a terminal and running **tar -zxvf
    \<filename\>** where \<filename\> is replaced with the name of the
    file. 
4.  Next find the folder name “SFML-2.5.1” this may be in the folder
    you’re working in or it may be in a subfolder with the name of the
    SFML .zip file. Once you have found it, move it and the unzipped
    folder containing the repo to wherever you would like to keep them.
    Simply ensure the layout of folders is maintained.
5.  Ensure you have g++ and make installed, this can be done by running
    **g++ --version** and **make --version** respectively. If you
    receive an error message about these commands not being found you
    will have to install them. On a debian based system such as Ubuntu
    Debian or Mint these can be installed with the command **sudo
    apt-get install g++ make **If you are not on a debian based system
    you should google the command for your package manager and install
    g++ and make using that.
6.  Next navigate to the AVC\_robot folder, it can be found inside the
    extracted folder from the repo zip. Open a terminal in this folder
    and run the **make** command. If you receive an error double check
    the SFML -2.5.1 folder is two levels up from the AVC\_robot folder
    and that you have the correct version of SFML (2.5.1). If it says
    “robot is up to date” delete the robot file and run **make**
    again. Leave a terminal open in this folder.
7.  Navigate up to the AVC\_linux\_server folder, you can edit the first
    line of the config.txt file to choose which maze you want to test
    the robot in, replace the everything after **mazeFile, ** with the
    file name of the maze you want to test leaving no spaces on the
    line. 
8.  You should be able to start the server by running **./server3** if
    that doesn’t work try running the **make** command or setting the
    server3 file to be executable, probably with the command **chmod +x
    server3** although this may differ for your linux distro.
9.  Once the server window appears, pull up the terminal window you left
    open in the AVC\_robot folder and run **./robot **If this doesn’t
    work try running **chmod +x robot** or the equivalent command for
    your distro.
10. The robot should now be navigating your maze of choice.

<span id="anchor-1"></span>A few tips on non-linux OS’s NOTE: Not a
complete guide

  - The makefile for the robot should operate on Mac, Windows and Linux,
    the non linux server should run on Windows and Mac but the makefile
    won’t, it’s mac only.
  - The linux robot makefile requires the SFML-2.5.1 folder one level up
    from the server and robot folders, the mac one requires SFML in a
    folder in /usr/local/lib/SFML, windows requires SFML in a folder at
    C:\\SFML\\ In all cases the SFML folder should contain the lib,
    include and share folders directly with no other folders before
    them.

<span id="anchor-2"></span>AVC Plan

**Team Name: Team 15**

**Team Members & contact info:**

Julius Rieser

Lily Holder-McFlinn

Daniel Southward-Ellis

Conrad Draper (Missing)

<span id="anchor-3"></span>**Communication tool:**

Google doc, Discord, VUW Email, Gmail

<span id="anchor-4"></span>**Roles:**

Lily - Project Manager (organising team meetings, reporting regularly on
progress)

Daniel - Software Architect (writing core code and extending
functionality)

Lily - Software writing, testing and documentation (debugging software
and committing to

git, writing test cases and documenting performance against milestones)

Julius - Software writing, testing and documentation (debugging software
and committing to

git, writing test cases and documenting performance against milestones)

Github:
[*https://github.com/Daniel-Southward-Ellis/ENGR\_101\_Project3*](https://github.com/Daniel-Southward-Ellis/ENGR_101_Project3)

<table>
<tbody>
<tr class="odd">
<td>Date</td>
<td>Objectives</td>
<td>Due date</td>
<td>Item due</td>
<td>Conflicts</td>
<td>Tasks</td>
</tr>
<tr class="even">
<td>3rd of June</td>
<td><strong>Start the Project</strong></td>
<td>4th of June</td>
<td>Team log</td>
<td>Unable to locate 4th team member</td>
<td><p>All of the team works together to plan what key objectives we need to achieve in the next fortnight. </p>
<p>Key milestones are determined to create a ‘simple’ functioning robot (core and completion), and to create an ‘advanced’ robot capable of navigating corridors</p>
<p>Figure out the respective roles of each team member in the project.</p>
<p>Try and get in touch with our other team member before the roles are finalised. </p>
<p>Daniel - Help anyone who is having issues with installing the software on their machines.</p>
<p>Lily - Ensures that everyone has contributed to the team plan.</p>
<p>Julius - Make sure everyone can successfully compile the program, and that everyone’s environments are working as planned.</p>
<p>All of the team - do some test commits to make sure everything in the repo is functioning correctly before we start programming (environments are configured correctly)</p></td>
</tr>
<tr class="odd">
<td> June 3rd</td>
<td><strong>Core/Completion - </strong>simple (make sure that the robot follows the white line; not including obstacles/barriers) </td>
<td>8 June</td>
<td>Core code</td>
<td>Assignments/tests from other courses</td>
<td><p><strong>Basic goal:</strong> Get the robot moving with limited functionality. Write some code for it to move around (not concerning the white line for now). </p>
<p>Lily and Julius - investigate how the program/robot works, and what functionality we need to implement to get the robot to move around (turn right, turn left, move straight). Figure out how the virtual motors work. </p>
<p>All of the team - discuss strategy for approaching simple movements so everyone has a clear idea going forward.</p>
<p>Daniel - attempt to implement the strategy. Make sure the robot moves as expected. Lily and Julius can help with bugs and/or confusion that arises with the algorithm. </p>
<p><strong>Goal: </strong>once the robot is able to move as we predict, implement functionality for the robot to follow the white line (straight, curved etc)</p>
<p>Lily and Julius - do some investigation into how we can detect the white line, and what movements we should make the robot do when the white line is sensed to be in a certain direction.</p>
<p>Team - discuss these findings, come to a consensus on the best approach. </p>
<p>Daniel - code a solution, with the help of Lily and Julius if required.</p>
<p>Lily - write tests and document changes. Help with debugging.</p>
<p>Julius - write tests as well, and fix bugs that arise from the testing phase.</p></td>
</tr>
<tr class="even">
<td> 8 June</td>
<td><strong>Core/Completion</strong> - extended (write tests for ensuring the robot <em>always </em>follows the white line; exceptional cases where objects are blocking the robot; make sure code works in most cases </td>
<td> 11 June</td>
<td>Completion Code &amp; testing documentation</td>
<td>Assignments from other courses</td>
<td><p><strong>Goal:</strong> Once we get the robot to follow the line we start testing how to avoid obstacles such as walls/offmap/ducks and what the robot has to do in each situation</p>
<p>Lily and Julius - do some investigation into how code detects various obstacles, and what movements we should make the robot do when the robot senses various obstacles.</p>
<p>Daniel - code a solution, from investigation. Lily and Julius will provide support and ideas if needed</p>
<p>Lily and Julius - write testing documentation on the results of what the robot does when obstacles are seen on the camera. Help with debugging</p></td>
</tr>
<tr class="odd">
<td>11 June</td>
<td><strong>Challenge</strong> (simple) - write new code for the robot to be able to travel down a corridor successfully</td>
<td>13 June</td>
<td>Basic Challenge code + Tests</td>
<td>Assignments/tests from other courses</td>
<td><p><strong>Goal: </strong>To have a system in place to allow the robot to navigate down a corridor or other enclosed space.</p>
<p>Team - Discuss possible options for navigating.</p>
<p>Daniel - Architect and write a system to track and navigate down a corridor. </p>
<p>Lily and Julius - Write tests for the system and determine situations where it breaks for the next step.</p></td>
</tr>
<tr class="even">
<td>13 June</td>
<td><strong>Challenge </strong>(complex) - write test cases for exceptional circumstances of corridor travelling. Make sure robot travels <em>correctly </em>even in complex scenarios (hardcore debugging)</td>
<td>16 June</td>
<td>Complex challenge code +Tests</td>
<td>Assignments/tests from other courses</td>
<td><p><strong>Goal: </strong>To ensure our navigation system doesn’t break in more complex situations. Multiple walls and corridors, objects in the way, etc.</p>
<p>Lily and Julius - Discuss cases each team member has found where the code doesn’t work as expected or doesn’t navigate correctly. Determine any additional cases that should be improved or that might cause issues.</p>
<p>Daniel - Work on the code to fix these issues, with the help of Lily and Julius if required.</p></td>
</tr>
<tr class="odd">
<td>16 June</td>
<td><strong>Extra testing phase </strong>(test core/completion and challenge with a range of test cases, simple to complex). At the end of this there should be no bugs</td>
<td>June 19</td>
<td>Final Code for all levels + Tests for all levels</td>
<td>Assignments/tests from other courses</td>
<td><p><strong>Goal: </strong>To ensure existing bugs are fixed and behavior is consistent.</p>
<p>Lily and Julius - Check and improve on existing tests, create new ones if necessary. Report any issues found.</p>
<p>Daniel - Fix bugs found in the code and improve any issues found with help from Lily and Julius.</p></td>
</tr>
<tr class="even">
<td>June 4</td>
<td><strong>Team log/ individual logs </strong>(compile all of our logs, make sure we have good documentation for all of our tasks and achievements)</td>
<td>June 19</td>
<td>Individual Logs, team logs</td>
<td>Forgetting to log, assignments from other courses</td>
<td><p><strong>Individual Log:</strong></p>
<p>All of the team - Make sure to log all activity on project work within individual logs with at least start time, finish time, task, and outcome.</p>
<p><strong>Team Log:</strong></p>
<p>All of the team - Make sure to keep as close to the plan as possible.</p>
<p>Lily - reports on team progress regularly with help if needed by Julius and Daniel.</p></td>
</tr>
<tr class="odd">
<td>June 19</td>
<td><p><strong>Team log/ individual logs/ Report</strong></p>
<p>Touch ups on individual and team logs if needed. Write a report to your manager on the software our team just completed</p></td>
<td>June 24</td>
<td>Individual Logs, team logs, and Report</td>
<td></td>
<td><p>All of the team - Make sure that all activity has been logged for individual/team logs and if anything is missing or unaccounted for, it will be added.</p>
<p><strong>Report:</strong></p>
<p>All of the team - Write a report that is directed to the manager about the software our team has just created. Should include:</p>
<p>Task, approach, architecture, solution, team dynamic, state of the art, to do, and recommendations. No more than 10 pages.</p></td>
</tr>
</tbody>
</table>
