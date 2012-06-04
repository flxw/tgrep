If you want to find files inside your huge collection of files
reasonably fast, **tgrep** may be worth the try.

For example, after trying out to sort software by tags, lots of albums
that were mixed up out of several songs, I wanted these songs to be in a single
directory again.

By using tgrep, that was done in a matter of seconds:

    $ mkdir Hypermix
    $ tgrep -Ro -r 'Hypermix' . | xargs -d '\n' -I '{}' mv '{}' Hypermix/
