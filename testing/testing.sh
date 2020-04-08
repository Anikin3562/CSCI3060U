#! /bin/bash

#login tests
for TEST in successful_login unsuccessful_logins
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing login/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/login/$TEST/input.txt >output.txt
	if ! cmp -s "tests/login/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/login/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done

#logout tests
for TEST in only_login_accepted_after_logout successful_logout
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing logout/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/logout/$TEST/input.txt >output.txt
	if ! cmp -s "tests/logout/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/logout/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done

#create tests
for TEST in admin_only invalid_user_types successful_create unique_user_names username_character_limit
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing create/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/create/$TEST/input.txt >output.txt
	if ! cmp -s "tests/create/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/create/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done

#delete tests
for TEST in admin_only cant_delete_self successful_delete unsuccessful_deletes
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

#addcredit tests
for TEST in addcredit_admin addcredit_admin_user_exists addcredit_max addcredit_standard
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing addcredit/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/addcredit/$TEST/input.txt >output.txt
	if ! cmp -s "tests/addcredit/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/addcredit/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done

#advertise tests
for TEST in auction_length_lower auction_length_upper has_permissions item_name_length mid_bid_upper min_bid_lower successful_advertise
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing advertise/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/advertise/$TEST/input.txt >output.txt
	if ! cmp -s "tests/advertise/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/advertise/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done

#bid tests
for TEST in bid_amount_constraints bidder_must_have_sufficient_funds bid_must_be_five_percent_higher_than_previous_highest_for_FS_and_SB bid_must_only_be_higher_than_previous_highest_for_admin cannot_bid_on_own_item standard_sell_not_allowed successful_bid unsuccessful_bids
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing bid/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/bid/$TEST/input.txt >output.txt
	if ! cmp -s "tests/bid/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/bid/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done

#refund tests
for TEST in admin_only invalid_usernames refund_amount_constraints seller_must_have_sufficient_funds successful_refund
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing refund/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/refund/$TEST/input.txt >output.txt
	if ! cmp -s "tests/refund/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/refund/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done

#other tests
for TEST in exit_only_works_when_logged_out no_transaction_allowed_before_login
do
	>output.txt #empty output.txt
	>transactions.txt #empty daily_transactions.txt
	echo performing other/$TEST test
	../auction_system accounts.txt items.txt transactions.txt <tests/other/$TEST/input.txt >output.txt
	if ! cmp -s "tests/other/$TEST/expected_output.txt" output.txt
	then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/other/$TEST/expected_transactions.txt" transactions.txt
	then
		echo "FAILED: transactions"
	fi
done
