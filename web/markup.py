# definition of dishMark parser
# we assume most inputs are **WELL-FORMED**
class dishMark():
    # trigger functions
    
    def bold(str):
        return '<b>' + str + '</b>'

    def ital(str):
        return '<i>' + str + '<i>'
    
    # special formatting of the form: [stuff to link](link)
    # if link starts with #, then we make a badge instead, with the specified color
    def link(str):
        # we find the end of the stuff, and the beginning of the link
        delimiter = str.find("](")

        if delimiter == -1:
            # ill-formed. quit.
            return str

        text = str[:delimiter]
        link = str[delimiter + 2:]

        if link[0] == "#":
            # it's a badge!
            badge = '<span class="badge badge-light" style="background-color: ' + \
                    link + \
                    ';">' + text + '</span>'
            return badge
        else: 
            # otherwise, we treat as link
            return '<a href=' + link + '>' + text + '</a>'
    
    # trigger characters 
    chars = {
        '*': bold,
        '_': ital,
        '[': link,
        ')': link
    }
    
    def parse(self, str):
        new_str = str
        trig_chars = {}
        changes = {}

        # find all triggers
        for i, ch in enumerate(str):
            if ch in self.chars:
                # we make note of it
                try:
                    # special treatment for links
                    if ch == ')':
                        trig_chars['['].append(i)
                    trig_chars[ch].append(i)
                except KeyError:
                    trig_chars[ch] = [i]
        
        for (trig, positions) in trig_chars.items():
            # if we have an odd numbner of triggers (that is, they aren't balanced), we remove the last one
            if (len(positions) % 2 == 1):
                positions.pop()
            
            # we loop through the trigger postions we found
            for i in range(0, len(positions), 2):
                start = positions[i] + 1
                end = positions[i + 1]

                # call our formatting function
                formatted = self.chars[trig](str[start:end])
                print(formatted)

                # record the changes
                changes[str[start - 1:end + 1]] = formatted
        # make changes
        for orig, new in changes.items():
            new_str = new_str.replace(orig, new)
        
        return new_str