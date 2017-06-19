# text-adventure-engine
A program that allow the creation of 'choose your own adventure' stories using XML

Program will prompt user to open a .xml file to run the story from.

XML Story Syntax:

    <start></start> The entry point for the story [manditory].
    <title></title> The title of the story [manditory].
    <settings></settings> The settings for the story [not manditory] (see Settings).
    <synopsis></synopsis> A brief description of the story [not manditory].

    <ln/> Will print a line break when printing text (may NOT be used in selections).
    <br/> Will pause the output and prompt the user to press enter (may NOT be used in selections).

    Tags nested one layer within another are treated as selections.
    When a selection is picked, control is passed to the coresponding
    global tag with the same name as the selection.

    A tag with no selections is treated as an exit point for the program
    and, as such, the program will end once the tag's text is printed.

    example:
    <start>
        Hello World!
        <selection_1>Hello</selection_1>
        <selection_2>Hey</selection_2>
    </start>

    <selection_1>This is displayed if selection 1 is picked</selection_1>

    <selection_2>This is displayed if selection 2 is picked</selection_2>


Settings:
    
    <text_delay>'int'</text_delay>                      Allows for cascading printing of the text if value is not 0.
    <display_synopsis>'bool'</display_synopsis>         Should display the contents of the <synopsis> tag when program starts.
    <synopsis_text_delay>'bool'</synopsis_text_delay>   Should print synopsis using cascading printing. (<text_delay> must not be 0).
