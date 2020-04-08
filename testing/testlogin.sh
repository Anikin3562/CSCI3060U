#! /bin/bash
for TEST in cant_delete_self
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing delete/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/delete/$TEST/input.txt >output.txt
	if ! cmp -s "tests/delete/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/delete/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done
