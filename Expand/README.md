# Expand

More parsing. Given a sequence of words as input, react to 'DEFINE' as as a command declaring intention to make the following word into the upcoming phrase (which ends before the commmand STOP).

Ex: DEFINE bob a man, a cheery individual, jibberish STOP

Every instance of the defined words after the DEFINE declaration should be expanded into its definition. No recursive definitions exist. Expect memory overflow if one attempts to store the entire sequence of words.