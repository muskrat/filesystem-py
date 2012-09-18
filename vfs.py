import string

#TODO: paths, how to represent them?

class Inode(object):
    """A node in the filesystem.

    In a real filesystem this would probably contain a pointer to the
    disk block associayed with it, but this isn't a real filesystem.
    """
    def __init__(self, ID, ftype, data=""):
        self.ID = ID
        self.ftype = ftype
        self.data = data

    def __str__(self):
        return self.ftype + '\n' + self.data

    def append(self, data):
        self.data += data


class InodeTable(object):
    """Manage allocation and access to inodes."""
    def __init__(self):
        self.unallocated = 0
        self.table = {}

    def alloc(self, ft):
        """Create a new inode."""
        i = self.unallocated
        self.table[i] = Inode(i, ftype=ft)
        self.unallocated += 1
        return self.table[i]

    def get(self, ID):
        return self.table[ID]


class System(object):
    """Interface to the virtual filesystem."""
    def __init__(self):
        self.itable = InodeTable()
        self.root = self.itable.alloc('d')
        self.current = self.root

    def mkdir(self, name):
        """Create a new directory within current."""
        current = self.current
        new = self.itable.alloc('d')
        entry = format_entry(name, new.ID)
        current.append(entry)

    def chdir(self, dirname):
        """Attempt to switch to given subdirectory."""
        if dirname == '/':
            self.current = self.root
        subdir_ID = parse(self.current.data, dirname)
        if subdir_ID:
            self.current = self.itable.get(subdir_ID)

    def listdir(self):
        """List contents of current directory"""
        print self.current

    # TODO: move parse into a directory class? Add error handling
    def lookup(self, path):
        """Return the inode at given path."""
        if path [0] == path_sep:
            # Absolute path
            current = self.root 
        else:
            # Relative path
            current = self.current

        elements = filter_split(path, path_sep)
        for element in elements:
            current = self.itable.get(parse(current.data, element))
        return current


"""
Define the directory entry format and path format here for easy 
modification, who knows why you'd want to though...
"""
path_sep = '/'
node_sep = ':'
entry_sep = '\n'

def format_entry(name, ID):
    """Return name and ID in directory entry format"""
    return name + node_sep + str(ID) + entry_sep

def parse(data, name):
    """Return inode number associated with given name in data or None"""
    # Filter out empty strings
    entries = filter_split(data, entry_sep)
    for entry in entries:
        filename, inode = entry.split(node_sep)
        if filename == name:
            return int(inode)
    return None

def filter_split(s, sep):
    """Split given string and remove empty strings from result."""
    return filter(lambda x: x != "", s.split(sep))

def main():
    s = System()

