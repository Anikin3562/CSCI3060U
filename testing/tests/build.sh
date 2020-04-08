#! /bin/bash

cd login  
for TEST in successful_login unsuccessful_logins
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..

cd logout
for TEST in successful_logout only_login_accepted_after_logout
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..

cd create
for TEST in successful_create unsuccessful_creates username_character_limit unique_user_names credit_limit no_negative_credit admin_only
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..

cd delete
for TEST in successful_delete unsuccessful_deletes cant_delete_self admin_only
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..

cd advertise
for TEST in successful_advertise unsuccessful_advertises standard_buy_not_allowed price_constraints item_name_character_limit days_for_auction_constraints
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..

cd bid
for TEST in successful_bid unsuccessful_bids standard_sell_not_allowed bidder_must_have_sufficient_funds bid_must_be_five_percent_higher_than_previous_highest_for_FS_and_SB bid_must_only_be_higher_than_previous_highest_for_admin bid_amount_constraints cannot_bid_on_own_item
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..

cd refund
for TEST in successful_refund unsuccessful_refunds admin_only refund_amount_constraints reject_when_refund_increases_user_credit_beyond_limit seller_must_have_sufficient_funds
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..

cd addcredit
for TEST in admin_successful_addcredit admin_unsuccessful_addcredits admin_credit_limit non_admin_successful_addcredit non_admin_unsuccessful_addcredits non_admin_credit_limit
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..

cd other
for TEST in no_transaction_allowed_before_login exit_only_works_when_logged_out
do
	cd $TEST
	touch expected_output.txt
	cd ..
done
cd ..
