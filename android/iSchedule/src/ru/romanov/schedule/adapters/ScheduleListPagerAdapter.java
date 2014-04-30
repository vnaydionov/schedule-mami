package ru.romanov.schedule.adapters;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import ru.romanov.schedule.src.ScheduleListFragment;

/**
 * Created with IntelliJ IDEA.
 * User: nikita
 * Date: 13.05.13
 * Time: 22:01
 * To change this template use File | Settings | File Templates.
 */
public class ScheduleListPagerAdapter extends FragmentPagerAdapter {


    public ScheduleListPagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        return ScheduleListFragment.newInstance(position);
    }

    @Override
    public int getCount() {
        return 20;
    }

}
