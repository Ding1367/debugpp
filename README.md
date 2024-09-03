# debug++
Additional debugging features for Lua 5.x.

# Metatables
Because Lua 5.0 doesn't have a setmetatable and getmetatable function in debug, debug++ contains a way to do that. You can also get a metatable with a null terminated string which is the name. This is a global metatable for a certain type. The function to get a global metatable is getgmetatable.

# Externalization
debug++ contains some method that makes some internal functions external, like isnone, which is essentially a wrapper for the C call *lua_isnone(L, 1)*.

You can also get userdata metatables, like FILE \*. Oh, and you can also get the so-called inaccessible to Lua table, the registry too (because Lua 5.0 doesn't have the getregistry function). Just use getregistry.

There's also an available getfd(file) function and a close(file/fd) function. What these do is ignore Lua boundaries and just do what it's asked to, if it fails it will return the message. However, you can't use close(fd) on Windows.

# Explaining none
In case you don't know what a none is, it's what's after the top of the Lua stack structure.

This means if the top of the stack, or the index the next value will be at, is 94, *lua_isnone(L, 94)* or checking with any value greater than 94, ti will return true.

Now you may be wondering, wait so if I pass in the return value of print (none, not nil) into print, why does it say nil? That's because the tostring to an inaccessible or invalid stack index (where nones live), is the same as the tostring for a nil. In fact, in Lua when you try to refer to none without using an external way to handle none, like this library, it just replaces it with nil. So if you check if nil == print() or some function that returns 0 return values, it will be true because the return value in that context is replaced with nil.

# Memory
You can allocate a chunk of memory using create(size), readfrom(it), and writeto(it, value). You can also getnum(it) to get the number and getptr(it) to get the address at the number you fed into getptr. This DOES mean you can manually handle memory and crash your program, but you see that's just not my problem. This also means you can write to overwrite memory you technically, and I mean *technically*, don't own but whatever.

Little note, make sure you implement writefloat and writeint and stuff, because readfrom and writeto read byte by byte. However, you can use getnum and getptr to just implement this yourself.

# OS stuff
Not exactly stuff, you can only check if the operating system is Windows or not, and also check if it's 64 bit or 32 bit. The functions are iswindows and archsize. *archsize* in particular doesn't actually return if it's 64-bit or 32-bit, it returns the size of a pointer times 8, to infer the architecture size.
