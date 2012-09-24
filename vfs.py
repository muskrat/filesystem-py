import string

# TODO: All functions that operate on directory entries need to be methods
#       in this or an inherited class
class Inode(object):
    """A node in the filesystem.

    In a real filesystem this would probably contain a pointer to the
    disk block associated with it, but this isn't a real filesystem.
    In the Unix file system inodes store metadata about the file. Here
    we only store file type: 'd' for directory and 'f' for file.
    """
    def __init__(self, ID, ftype, data=""):
        self.ID = ID
        self.ftype = ftype
        self.data = data

    def __str__(self):
        return self.ftype + '\n' + self.data

    def append(self, data):
        self.data += data

    def is_dir(self):
        if self.ftype == 'd':
            return True
        else:
            return False


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

    def dealloc(self, ID):
        """Delete inode entry at given node"""
        del self.table(ID)

    def get(self, ID):
        """Return inode object at given index"""
        return self.table[ID]


# TODO: appends of entries to directories need uniform interface
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

    def chdir(self, path):
        """Attempt to switch to given subdirectory."""
        result = self.lookup(path)
        if result.is_dir():
            self.current = result

    def new_file(self, filename):
        """Create a new file in current directory."""
        new = self.itable.alloc('f')
        entry = format_entry(filename, new.ID)

    def delete_file(self, filename, del_inode=False):
        """Delete given file from current directory, optionally removing inode"""
        entries = self.current.split

    def move(self, source_path, dest_path):
        """Move a file from source directory to dest directory."""
        inode = self.lookup(source_path)
        
        # Delete file entry from source dir
        # Create file entry in dest dir

    def listdir(self):
        """List contents of current directory"""
        print self.current

    # TODO: . and .. special files
    def lookup(self, path):
        """Return the inode object at given path."""
        if path [0] == PATH_SEP:
            # Absolute path
            current = self.root 
        else:
            # Relative path
            current = self.current

        elements = filter_split(path, PATH_SEP)
        for element in elements:
            current = self.itable.get(parse(current.data, element))
        return current


"""
Define the directory entry format and path format here for easy 
modification, who knows why you'd want to though...
"""
PATH_SEP = '/'
NODE_SEP = ':'
ENTRY_SEP = '\n'

def format_entry(name, ID):
    """Return name and ID in directory entry format"""
    return name + NODE_SEP + str(ID) + ENTRY_SEP

def parse(data, name):
    """Return inode number associated with given name in given dir or None"""
    # Filter out empty strings
    entries = filter_split(data, ENTRY_SEP)
    for entry in entries:
        filename, inode = entry.split(NODE_SEP)
        if filename == name:
            return int(inode)
    return None

def filter_split(s, sep):
    """Split given string and remove empty strings from result."""
    return filter(lambda x: x != "", s.split(sep))


class Interface(object):
    """Provides the command-line interface to the file system"""
    def __init__(self):
        self.fs = System()

    def repl(self):
        """Command interpreter."""
        running = Trueself.links
        while running:
            cmd = raw_input('> ')
            if cmd == "quit":
                running = False
