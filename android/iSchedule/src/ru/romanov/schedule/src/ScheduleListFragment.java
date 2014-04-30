package ru.romanov.schedule.src;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ExpandableListView;
import ru.romanov.schedule.R;
import ru.romanov.schedule.adapters.SubjectAdapter;
import ru.romanov.schedule.adapters.SubjectsListAdapter;
import ru.romanov.schedule.utils.Subject;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Locale;

/**
 * Created with IntelliJ IDEA.
 * User: nikita
 * Date: 15.05.13
 * Time: 13:24
 * To change this template use File | Settings | File Templates.
 */
public class ScheduleListFragment extends Fragment{
    static final String PAGE_NUMBER = "page_number";

    int pageNumber;
    private ArrayList<ArrayList<Subject>> subjList;
    private ArrayList<ArrayList<String>> group;
    private HashMap<String, String> ruDays = new HashMap<String, String>();

    static public ScheduleListFragment newInstance(int page) {
        ScheduleListFragment scheduleListFragment = new ScheduleListFragment();
        Bundle arguments = new Bundle();
        arguments.putInt(PAGE_NUMBER, page);
        scheduleListFragment.setArguments(arguments);
        return scheduleListFragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        pageNumber = getArguments().getInt(PAGE_NUMBER);

        ruDays.put("Mon", "Пн");
        ruDays.put("Tue", "Вт");
        ruDays.put("Wed", "Ср");
        ruDays.put("Thu", "Чт");
        ruDays.put("Fri", "Пт");
        ruDays.put("Sat", "Сб");
        ruDays.put("Sun", "Вс");
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle saveInstanceState) {
        View view = inflater.inflate(R.layout.list_activity_fragment, null);
        ExpandableListView scheduleList = (ExpandableListView) view.findViewById(R.id.scheduleList);

        loadData();

        SubjectsListAdapter subjectListAdapter = new SubjectsListAdapter(this.getActivity(), group, subjList);
        scheduleList.setAdapter(subjectListAdapter);


        return view;
    }

    private void loadData() {
        group = new ArrayList<ArrayList<String>>();
        subjList = new ArrayList<ArrayList<Subject>>();

        SubjectAdapter sa = new SubjectAdapter(this.getActivity());

        Calendar calend = Calendar.getInstance(Locale.FRANCE);

        SimpleDateFormat sdf = new SimpleDateFormat("dd.MM.yyyy");
        calend.add(Calendar.DAY_OF_YEAR, pageNumber*7);
        for (int i = 0; i < 7; ++i) {
            ArrayList<String> tmp = new ArrayList<String>();
            calend.set(Calendar.DAY_OF_WEEK, calend.getFirstDayOfWeek() + i);
            String disp_name = calend.getDisplayName(Calendar.DAY_OF_WEEK, Calendar.SHORT, Locale.ENGLISH);
            tmp.add(sdf.format(calend.getTime()));
            tmp.add(ruDays.get(disp_name));
            group.add(tmp);

            subjList.add(sa.getSubjectsByDate(disp_name));
        }
    }
}
