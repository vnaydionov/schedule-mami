package ru.romanov.schedule.adapters;

import ru.romavon.fragments.ScheduleListTabFragment;
import ru.romavon.fragments.UpdateListTabFragment;
import ru.romavon.fragments.WeeklyListTabFragment;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.util.Log;

public class TabCollectionPagerAdapter extends FragmentPagerAdapter {

	private final int NUM_ITEMS= 3;
	
	public TabCollectionPagerAdapter(FragmentManager fm) {
		super(fm);
	}

	@Override
	public Fragment getItem(int arg0) {
		Fragment fragment = new Fragment();
		switch(arg0) {
			case 0:
				fragment = new WeeklyListTabFragment();
				break;
			case 1:
				fragment = new ScheduleListTabFragment();
				break;
			case 2:
				fragment = new UpdateListTabFragment();
				break;
			default:
				Log.e(getClass().getSimpleName(), "Not valid tab index");
		}
		Bundle args = new Bundle();
		fragment.setArguments(args);
		return fragment;
	}


	@Override
	public int getCount() {
		return NUM_ITEMS;
	}
	
}