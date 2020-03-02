#! /bin/bash

#login tests
for TEST in successful_login unsuccessful_logins
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing login/$TEST test
	../auction_system accounts.txt items.txt <tests/login/$TEST/input.txt >output.txt
	if ! cmp -s "tests/login/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/login/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done

#logout tests
for TEST in only_login_accepted_after_logout
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing logout/$TEST test
	../auction_system accounts.txt items.txt <tests/logout/$TEST/input.txt >output.txt
	if ! cmp -s "tests/logout/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/logout/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done

#create tests
for TEST in admin_only invalid_user_types successful_create unique_user_names username_character_limit
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing create/$TEST test
	../auction_system accounts.txt items.txt <tests/create/$TEST/input.txt >output.txt
	if ! cmp -s "tests/create/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/create/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done

#delete tests
for TEST in admin_only cant_delete_self successful_delete unsuccessful_deletes
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing delete/$TEST test
	../auction_system accounts.txt items.txt <tests/delete/$TEST/input.txt >output.txt
	if ! cmp -s "tests/delete/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/delete/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done

#addcredit tests
for TEST in addcredit_limit admin_successful_addcredit non_admin_successful_addcredit unsuccessful_addcredits
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing addcredit/$TEST test
	../auction_system accounts.txt items.txt <tests/addcredit/$TEST/input.txt >output.txt
	if ! cmp -s "tests/addcredit/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/addcredit/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done

#advertise tests
for TEST in days_for_auction_constraints item_name_character_limit price_constraints standard_buy_not_allowed successful_advertise
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing addcredit/$TEST test
	../auction_system accounts.txt items.txt <tests/addcredit/$TEST/input.txt >output.txt
	if ! cmp -s "tests/addcredit/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/addcredit/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done

#bid tests
for TEST in bid_amount_constraints bidder_must_have_sufficient_funds bid_must_be_five_percent_higher_than_previous_highest_for_FS_and_SB bid_must_only_be_higher_than_previous_highest_for_admin cannot_bid_on_own_item standard_sell_not_allowed successful_bid unsuccessful_bids
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing bid/$TEST test
	../auction_system accounts.txt items.txt <tests/bid/$TEST/input.txt >output.txt
	if ! cmp -s "tests/bid/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/bid/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done

#refund tests
for TEST in admin_only invalid_usernames refund_amount_constraints seller_must_have_sufficient_funds successful_refund
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing refund/$TEST test
	../auction_system accounts.txt items.txt <tests/refund/$TEST/input.txt >output.txt
	if ! cmp -s "tests/refund/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/refund/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done

#other tests
for TEST in exit_only_works_when_logged_out no_transaction_allowed_before_login
do
	>output.txt #empty output.txt
	>daily_transactions.txt #empty daily_transactions.txt
	echo performing other/$TEST test
	../auction_system accounts.txt items.txt <tests/other/$TEST/input.txt >output.txt
	if ! cmp -s "tests/other/$TEST/expected_output.txt" output.txt; then
		echo "FAILED: console output"
	fi
	if ! cmp -s "tests/other/$TEST/expected_transactions.txt" output.txt; then
		echo "FAILED: transactions"
	fi
done
