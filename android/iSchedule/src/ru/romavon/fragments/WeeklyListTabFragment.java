package ru.romavon.fragments;

import ru.romanov.schedule.R;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;

public class WeeklyListTabFragment extends Fragment {
	private ProgressBar progressBar;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.weekly_fragment_layout, null);
		progressBar = (ProgressBar) view.findViewById(R.id.weeklyProgressBar);

		return view;
	}

}
