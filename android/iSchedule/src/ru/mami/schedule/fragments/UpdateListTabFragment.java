package ru.mami.schedule.fragments;

import java.util.ArrayList;

import ru.mami.schedule.R;
import ru.mami.schedule.adapters.ScheduleUpdateListAdapter;
import ru.mami.schedule.adapters.SubjectAdapter;
import ru.mami.schedule.utils.Subject;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ListView;

public class UpdateListTabFragment extends Fragment implements OnClickListener {
    ListView listView;

    SubjectAdapter subjectAdapter;
    ScheduleUpdateListAdapter updateListAdapter;

    ArrayList<Subject> subjects;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        subjectAdapter = new SubjectAdapter(getActivity());
        subjects =  subjectAdapter.getNewSubjects();
        updateListAdapter = new ScheduleUpdateListAdapter(getActivity(), subjects);
    }


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.update_fragment_layout, null);
        listView = (ListView) view.findViewById(R.id.checkList);
        Button confirmButton = (Button) view.findViewById(R.id.check_confirm_button);

        listView.setAdapter(updateListAdapter);
        confirmButton.setOnClickListener(this);
        return view;
    }


    @Override
    public void onClick(View v) {
        switch (v.getId()){
        case R.id.check_confirm_button:
            break;
        }
    }
}
