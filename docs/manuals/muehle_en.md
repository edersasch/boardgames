# Muehle (Nine Men's Morris)

Translated from german [Wikipedia](https://de.wikipedia.org/wiki/M%C3%BChle_(Spiel)#Spielablauf), version from 29th July 2018:

> The game has three stages:
>
> * Setting stage: The players alternately set one of the nine pieces on
> junctions or corners of the board.
>
> * Move stage: A piece gets moved to an adjacent free field. A player has
> lost if he cannot move a piece.
>
> * Final stage: If a player only has three pieces left, a piece can be moved
> to any free field. As soon as another piece gets removed, the game is lost.
>
> Three pieces of the same color that lie in a straight line next to each
> other are called a "mill". If a player closes a mill, he may remove an
> opponent's piece, if it is not part of a mill itself. The Official rules
> allow since 2010 that a piece can get removed from a mill if all opponent's
> pieces are part of a mill.

Official Rules: <http://www.muehlespiel.eu/images/pdf/WMD_Spielregeln.pdf>


# Game Features

* Depending on window or screen orientation a horizontal or vertical layout is used.
* The game is usable in small windows or screens.
* Pieces can be dragged to the destination or move there by clicking the destination.
* Setup mode to place pieces without rule checking is present.
* A move list can be made visible. According to the current layout it is shown on the right or at the bottom.
* Move lists can be imported and exported.
* For every player a computer opponent can be activated.
* Ending the game stores the session, starting the game restores it.


## Visual Clues

* Selectable pieces are bigger than the others
* The start field of the selected piece is green.
* Possible destinations of a selected piece show a transparent preview of the piece in the correct color.
* A piece than can be removed has a red background color.


# How to Operate

Starting the game restores the following states:

* [Move list](#move-list), its [visibility](#show-hide-move-list) and last position
* [Setup mode](#setup-mode)
* [Visibility of game info](#show-hide-game-info) ([time per player](#current-spent-time-per-player) starts at `0:00`.)
* [Computer opponent settings](#computer-opponent-settings) ([computer opponents](#computer-opponent) are off.)
* [Colors](#slider-for-color-adjustment)

If the pieces are not in home position, the move list counts as altered and
the actions

* [new game](#new-game)
* [setup mode](#setup-mode)
* [importing a move list](#importing-a-move-list)

have to be [confirmed](#confirm-cancel-chosen-action) from the beginning.

If a player has won, the color of his pieces starts to pulse.


## Making a Move

Selectable pieces are shown slightly bigger than the others. The start field of
the selected piece is colored green. If a field becomes green on its own, only
one piece was selectable.  Possible destination fields show a preview of the
piece. A move is made either by clicking on a piece and then clicking on the
destination field of by dragging a piece and dropping it on the destination. If
a mill has been closed, removable opponent pieces get a red background color.
Clicking on such a piece moves it to prison.


## Buttons Besides the Playfield


### New Game

![refresh](../../data/refresh.svg)<br>

First player's pieces land on their start fields to the left of the field in
the horizontal orientation. In the vertical orientation the start fields are
above the field. Second player's pieces are placed on the right or below the
field accordingly. If the move list has been modified, a
[confirmation](#confirm-cancel-chosen-action) is necessary, because the list
gets cleared on a new game. A possibly active [setup mode](#setup-mode) ends.
A possibly active [computer opponent](#computer-opponent) for the first player
gets deactivated.


### Computer Opponent

![engine](../../data/engine.svg)<br>

Every player has a button with the corresponding color. An activated computer
opponent has a green border around its button and lets the computer search for
the best next move. Activating both buttons makes the computer play for both
sides. Deactivating a button while the computer is searching interrupts the
search and the best move found up to this moment is made. Entering
[setup mode](#setup-mode) or choosing a move from the [move list](#move-list)
deactivates computer opponents. The other way round causes activating a
computer opponent the [setup mode](#setup-mode) to be left. A potentially
running computer opponent for the first player (left or top of the two buttons)
will be deactivated if a [new game](#new-game) is started.


### Show / Hide Move List

![list](../../data/list.svg)<br>

Controls visibility of the [move list](#move-list)


### Setup Mode

![setup](../../data/setup.svg)<br>

In setup mode every piece of every color can be selected and placed on every
free field except opponent start and prison fields. If the
[move list](#move-list) is modified, a
[confirmation](#confirm-cancel-chosen-action) is needed, because the list is
cleared on entering setup mode. Starting setup mode deactivates
[computer opponents](#computer-opponent). In setup mode no rules are checked.
So positions are possible that cannot be reached in a regular game. Take care
of what you are doing. Setup mode ends by pressing its button again or by
activating a [computer opponent](#computer-opponent). Starting a
[new game](#new-game) or [importing](#importing-a-move-list) a move list
needs a [confirmation](#confirm-cancel-chosen-action) before setup mode is
left, because the set up position gets lost. The player with the last selected
piece color is to move.


### Show / Hide Game Info

![gameinfo](../../data/info.svg)<br>

On the top the following information is shown:


#### Number of Moves to Current Position

A move consists of two half moves of the first and second player. So the
number changes in alternating play only after every second action. A hash
symbol `#` is prepended. The text `# 12` for example shows, that twelve
moves have already been played.


#### Current Spent Time per Player

A box with corresponding color the spent time in seconds is displayed. As long
as alternating moves are made as in a tournament situation the time tells who
took how long to get to the current position. This meaning for the game gets
lost in the following situations, which gets expressed by a
![warning](../../data/warning.svg) in front of the clocks:

* A former position gets chosen in the [move list](#move-list).
* [Setup mode](#setup-mode) is started.
* The game does not start in start position. Time per player still starts at `0:00`.

Starting a [new game](#new-game) sets the time for both players to `0:00` and the
potentially visible warning disappears.


#### Preview of Computer Opponent Result

When the computer opponent gets active, an additional box appears after the
timer box after latest one second that shows a preview of the result. On the
right of the computer opponent icon ![engine](../../data/engine.svg) follows
a box with the score of the current position:

* The background color corresponds to the player with the better score.
* The text is a plus sign and the score as a number.

The score is added up from these criteria:

* Each piece more in the game than the opponent: +100
* Each possible destination field for the own pieces more than the opponent: +5

`+230` for example is a very good score: The current player has two pieces and
six destination fields more than the opponent.

Then the symbol representing computation depth ![depth](../../data/depth.svg)
is shown with the reached depth as number behind. The last info is three fields
with one move each in the corresponding move color:

* The best next move the computer could find up to now
* The opponent's answer
* The answer for that from the current player


### Show / Hide Settings

![settings](../../data/settings.svg)<br>

Controls visibility of the [setting](#settings) shown at the top.


### Show Manual

![help](../../data/help.svg)<br>

Pressing the button shows this manual. In case doing this causes a problem the
version information is shown on top of the field until the button is pressed
again.


### Confirm / Cancel Chosen Action

![cancel](../../data/cancel.svg)<br>

If the move list was modified, the actions [new game](#new-game),
[setup mode](#setup-mode) and [import](#importing-a-move-list) need a
confirmation. [Deleting a variant](#delete-variant) always needs a
confirmation. The just pressed button will be moved to the left or down,
according to its neighbour buttons, and its color changes to red. The
former place has now the "cancel" button. To execute the desired action, the
moved button must be pressed again. Pressing the "cancel" button or waiting
for three seconds puts the button back in its place without triggering an
action.


# Move List

* Normal play adds buttons to the list. The last added button is automatically chosen.
* Choosing a move brings the pieces to the corresponding position. Potentially running [computer opponents](#computer-opponent) get deactivated.
* If a move is made, although the chosen move is not the last one in the list, a variant is made if the move differs from all successors. Otherwise an already existing move is chosen.
* Variants are slightly indented to their parent variant.
* Variants can be [deleted](#delete-variant) after a [confirmation](#confirm-cancel-chosen-action)
* If only one variant is left after deleting, its moves are appended to their parent variant.
* The list can be [imported](#importing-a-move-list) and [exported](#exporting-a-move-list).

The text in the buttons corresponds to [WMD notation (german)](http://muehlespieler.de/x_uebungen/index.php?page=begriffe_notation):
During setting stage onf the destination field is shown, e.g. `B4`. In the
move and final stage the origin and destination field are shown delimited by
a hyphen, e.g. `D1-D2`. If a piece is removed, the text ends with an `x` and
the field where the removed pices was last on. In the setting stage this looks
like e.g. `A1 xD2`, in the other stages e.g. `F4-F6 xD7`.


### Buttons on top of the Move List


#### Home Position

![home](../../data/home.svg)<br>

The pieces return to their start fields or to the fields set up during
[setup mode](#setup-mode). Potentially running
[computer opponents](#computer-opponent) are stopped.


#### Backwards / Forwards

![back](../../data/back.svg) ![forward](../../data/forward.svg)

Moves are flipped forwards and backwards, without leaving the chosen variant,
even if parent variants are entered and left. Potentially running
[computer opponents](#computer-opponent) are stopped.


#### Importing a Move List

![import](../../data/import.svg)<br>

A move list chosen in a select file dialog will be imported. If the move list
is modified or [setup mode](#setup-mode) is active, a
[confirmation](#confirm-cancel-chosen-action) is needed, because the
existing list or set up position gets lost.


#### Exporting a Move List

![export](../../data/export.svg)<br>

The move list is exported to a file chosen in a select file dialog.


### Buttons for Move List Variants


#### Delete Variant

![delete](../../data/delete.svg)<br>

Deleting needs a [confirmation](#confirm-cancel-chosen-action). If the to be
deleted variant contains the currently chosen move, the
[home position](#home-position) gets active.


#### Show / Hide Successor Moves

![show](../../data/show.svg) ![hide](../../data/hide.svg)

An eye is visible for variants with more than one move. Pressing the open
eye hides the moves. The button changes its icon to the closed eye. Pressing
this or the permanently visible first move reveals all moves and the button
shows the open eye icon again.


## Settings


### Computer Opponent Settings

![engine](../../data/engine.svg) ![depth](../../data/depth.svg)

The plus and minus buttons adjust the computing depth of the computer
opponent. The bigger the number, the better plays the computer by taking
more time. The computing time can increase much from one number to the next.
Is the game still quite fast at e.g. depth 12, switching to depth 13 can
extend the waiting time unacceptably. Computing time depends on the machine
the game runs on and on the current game position.

![engine](../../data/engine.svg) ![time](../../data/time.svg)

The plus and minus buttons adjust the maximum computing time in seconds,
to be seen at the "s" behind the number in the box. How good the found move
will be after that time depends on the reached depth and that depends on
the machine the game runs on and on the current game position.


### Slider for Color Adjustment

The upper slider adjusts the bright color for the first player, the lower
slider the dark colors for the second player. The colors are adjusted
immediately in the whole game.


# Links

* Info about the game and tournaments, with examples and training (german): <http://muehlespieler.de>
* Description of the computer based complete solution: <http://library.msri.org/books/Book29/files/gasser.pdf>


----

![Creative Commons License](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)

This work is licensed under a [Creative Commons Attribution-ShareAlike 4.0 International License](http://creativecommons.org/licenses/by-sa/4.0/).
