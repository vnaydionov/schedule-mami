package ru.romanov.schedule.src;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import ru.romanov.schedule.R;
import ru.romanov.schedule.adapters.ScheduleListPagerAdapter;

/**
 * Created with IntelliJ IDEA.
 * User: nikita
 * Date: 15.05.13
 * Time: 13:57
 * To change this template use File | Settings | File Templates.
 */
public class ScheduleListActivity extends FragmentActivity {

    ViewPager viewPager;
    PagerAdapter pagerAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.list_activity_layout);
        viewPager = (ViewPager) findViewById(R.id.pager);
        pagerAdapter = new ScheduleListPagerAdapter(getSupportFragmentManager());
        viewPager.setAdapter(pagerAdapter);

        viewPager.setOnPageChangeListener(new ViewPager.OnPageChangeListener() {

            @Override
            public void onPageSelected(int position) {

            }

            @Override
            public void onPageScrolled(int position, float positionOffset,
                                       int positionOffsetPixels) {
            }

            @Override
            public void onPageScrollStateChanged(int state) {
            }
        });

    }
}
