# GreenEngine

## Table of Contents

* [Contributing](#contributing)
  * [File Structure](#file-structure)
  * [Files and Naming](#files-and-naming)
  * [Classes Style](#classes-style)
  * [Variables and Methods](#variables-and-methods)
  * [Coding Style](#coding-style)
  * [Comment Style](#comment-style)
  * [Commits](#commits)
  * [Branches](#branches)
  * [Pull Requests](#pull-requests)
  * [Issues](#issues)
  * [Contributors](#contributors)
* [Comments](#comments)

### Contributing

Contributing is key in this project. Any issues will be handled by the Redmine
Master (@jjanzen). Please email or label any issues are to be labelled in
[Redmine](http://btechgames.bcit.ca/redmine/projects/term3-comp8551-bestaround?jump=welcome)
and labelled accordingly, (see [Issues](#issues) for details).
Please follow all rules listed, and issues can be resolved quickly and
efficiently for such a short project timeline.

#### File Structure

The file structure will be as follows:

```cpp
race/
    - Game Engine/
        - Header Files/
        - Source Files/
        - etc..
    - Header Files/
    - Resource Files/
        - Libraries/
        - Models/
        - Shaders/
    - Source Files/
```

Feel free to make subfolders in the `Resource Files/` for assets, but just be
aware that they should follow some sort of style and logical sense.

#### Files and Naming

Files are to be given Upper Camel Case names.
>*Example*: MessageSystem.h
>*Example*: MessageSystem.cpp
>**Note**: main.cpp/main.h is the only execption to this rule

#### Classes Style

Classes are going to be given Upper Camel Case.
>*Example*: MessageSystem

#### Variables and Methods

Naming of class variables are to be given Underscore Lower Camel Case names.
>*Example*: Local Variable: `nameString`
>*Example*: Global Variable: `g_nameString`
>*Example*: Class Variable: `_nameString`
>*Example*: Local Pointer: `nameString_p`
>*Example*: Global Pointer: `g_nameString_p`
>*Example*: Class Pointer: `_nameString_p`

Naming of methods/functions are to be given Lower Camel Case.
>*Example*: setString

Naming for static and constants are to be given Upper Case.
>*Example*: `STATICVAR`
>*Example*: `SOMETHING_CONST`

#### Coding Style

Please use the [Allman](https://en.wikipedia.org/wiki/Indent_style#Allman_style)
Coding style conventions:

```cpp
while (x == y)
{
    something();
    somethingElse();
}
```

#### Comment Style

Comments are important so we know what functions are. Try if you can to use XML
style comments, however because we are in a time limited contest, just single
line comments will work.

```cpp
// This is a single line comment in C++
```

```cpp
///
/// <summary>
/// Change the string varable _s to the given string.
/// </summary>
/// <param name="s">String to set as.</param>
/// <return>If the new string is set or not.</return>
///
bool setString(string s)
{
    return _s = s;
}
```

#### Commits

Commit messages are important. It's a quick "here's what's up and what I did"
without having to read through all the changes and figure out what happened.
Please use the first 140 characters of the commit message as a message for what
the main changes in the commit were, and the rest of the message can be a bit
more detailed. Paragraphs are not expected at all, just a little bit of
information goes a long way.

#### Branches

Branching. One of the best features of Git. Please, please, please, branch
using the format below. It will keep everything clean, and make sure the files
you are working on are not being touched by anyone else on the same branch. It
helps merge conflicts immensly.

* `feature/[type_of_feature]`: Use `feature` branches for any new updates or

changes being added to the project.
>*Example*: `feature/engine`

* `hotfix/[issue number from Redmine]`: Use `hotfix` as a fix for any Redmine

>*Example*: `hotfix/151`
fixes or hotfixes.

Sticking to proper branching a lot of things can go right.
>Default Branches are: master and develop

#### Pull Requests

Pull requests are how changes are going to be merged into master.
> **Please do not push to `master` directly as it can cause a lot of issues and
we are only to push our final updates or releases to `master` branch**

When making a pull request, type out a message of what was changed overall,
what was worked on, any bug issues (using Redmine and labelling issue tags)
that arose during development.

#### Contributors

Team members include:

* Erick Fernandez de Arteaga
* John Janzen
* Chris Leclair
* Spencer Pollock
* Raj Singh

### Comments

Communication is key and keeping ontop of it is what leads to succesful
projects. Any quick questions or comments are done in Slack. Email and text is
always valid. Just make sure to update the group on any changes or what is
going to be worked on by individuals at any time.

***Good luck and have fun!***```cpp