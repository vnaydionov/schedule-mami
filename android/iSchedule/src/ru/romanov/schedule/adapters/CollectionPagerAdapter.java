package ru.romanov.schedule.adapters;

import ru.romanov.schedule.src.ScheduleListFragment;
import ru.romanov.schedule.src.UpdateListFragment;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

public class CollectionPagerAdapter extends FragmentPagerAdapter {

	private final int NUM_ITEMS= 2;
	
	public CollectionPagerAdapter(FragmentManager fm) {
		super(fm);
	}

	@Override
	public Fragment getItem(int arg0) {
		Fragment fragment = new Fragment();
		switch(arg0) {
			case 0:
				fragment = new ScheduleListFragment();
				break;
			case 1:
				fragment = new UpdateListFragment();
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