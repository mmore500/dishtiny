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

            # determine brightness to decide background color
            # based on https://stackoverflow.com/a/3943023
            (r, g, b) = (link[1:3], link[3:5], link[5:7])
            colors = []
            for c in (r, g, b):
                c = int(c, 16)
                if (c <= 0.03928):
                    colors.append(c / 12.92)
                else:
                    colors.append(((c + 0.055)/1.055) ** 2.4)
            L = 0.2126 * colors[0] + 0.7152 * colors[1] + 0.0722 * colors[2]

            if L > 0.179:
                type = "badge badge-dark"
            else:
                type = "badge badge-light"

            # for color string
            color = "background-color: " + link + ";"

            # form badge!
            badge = '<span class="' + type + '" style="' + color + '">' + text + '</span>'
            
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