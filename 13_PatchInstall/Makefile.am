bin_PROGRAMS=guess
pkgdata_DATA=ru/LC_MESSAGES/guess.mo
pkgdatadir=$(localedir)/ru/LC_MESSAGES
AM_CFLAGS = -D LOCALEDIR='"$(localedir)"'

ru/LC_MESSAGES/guess.mo: ru.po
	mkdir -p `dirname $@`
	msgfmt $< -o $@

ru.po: messages.pot
	msgmerge -U $@ $<

messages.pot: guess.pot guess.c
	xgettext -k_ -c guess.c -o messages.new.pot
	cp guess.pot messages.pot
	msgmerge -U messages.pot messages.new.pot

.PHONY: clean
clean:
	rm -f messages.new.pot messages.pot ru/LC_MESSAGES/guess.mo
