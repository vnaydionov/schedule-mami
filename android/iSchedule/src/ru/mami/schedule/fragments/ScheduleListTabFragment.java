package ru.mami.schedule.fragments;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Locale;

import ru.mami.schedule.R;
import ru.mami.schedule.adapters.SubjectAdapter;
import ru.mami.schedule.adapters.SubjectsListAdapter;
import ru.mami.schedule.utils.Subject;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ExpandableListView;


public class ScheduleListTabFragment extends Fragment {

    int pageNumber;
    private ArrayList<ArrayList<Subject>> subjList;
    private ArrayList<ArrayList<String>> group;
    private HashMap<String, String> ruDays = new HashMap<String, String>();

    static public ScheduleListTabFragment newInstance(int page) {
        ScheduleListTabFragment scheduleListFragment = new ScheduleListTabFragment();
        Bundle arguments = new Bundle();
        scheduleListFragment.setArguments(arguments);
        return scheduleListFragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

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
        View view = inflater.inflate(R.layout.schedule_fragment_layout, null);
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
