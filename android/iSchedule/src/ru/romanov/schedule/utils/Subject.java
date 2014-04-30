package ru.romanov.schedule.utils;

import java.util.Map;

public class Subject {
	private String place;
	private String checked = "false";
	private String subject;
	private String period;
	private String dt_start;
	private String dt_end;
	private String dow;
	private String t_start;
	private String t_end;
	private String groups;
	private String mode;
	private String id;
    private String activities;
	
	// Constructor using fields
	public Subject(String place, String checked, String subject, String period,
			String dt_start, String dt_end, String dow, String t_start,
			String t_end, String groups, String mode, String id, String activities) {
		super();
		this.place = place;
		this.checked = checked;
		this.subject = subject;
		this.period = period;
		this.dt_start = dt_start;
		this.dt_end = dt_end;
		this.dow = dow;
		this.t_start = t_start;
		this.t_end = t_end;
		this.groups = groups;
		this.mode = mode;
		this.id = id;
        this.activities = activities;
	}
	
	
	// Constructor from Map
	public Subject(Map<String, String> fields) {
		super();
		this.place = fields.get("place");
		this.checked = fields.get("checked");
		this.subject = fields.get("subject");
		this.period = fields.get("repeat");
		this.dt_start = fields.get("dt_start");
		this.dt_end = fields.get("dt_end");
		this.dow = fields.get("dow");
		this.t_start = fields.get("t_start");
		this.t_end = fields.get("t_end");
		this.groups = fields.get("groups");
		this.mode = fields.get("mode");
		this.id = fields.get("id");
        this.activities = fields.get("activities");
	}
	
	// Getters
	public String getId() {
		return id;
	}

	public String getPlace() {
		return place;
	}
	public String getChecked() {
		return checked;
	}
	public String getSubject() {
		return subject;
	}
	public String getPeriod() {
		return period;
	}
	public String getDt_start() {
		return dt_start;
	}
	public String getDt_end() {
		return dt_end;
	}
	public String getDow() {
		return dow;
	}
	public String getT_start() {
		return t_start;
	}
	public String getT_end() {
		return t_end;
	}
	public String getGroups() {
		return groups;
	}
	public String getMode() {
		return mode;
	}
    public String getActivities() {
        return activities;
    }
	
	// Setters
	public void setId(String id) {
		this.id = id;
	}
	
	public void setPlace(String place) {
		this.place = place;
	}
	public void setChecked(String checked) {
		this.checked = checked;
	}
	public void setSubject(String subject) {
		this.subject = subject;
	}
	public void setPeriod(String period) {
		this.period = period;
	}
	public void setDt_start(String dt_start) {
		this.dt_start = dt_start;
	}
	public void setDt_end(String dt_end) {
		this.dt_end = dt_end;
	}
	public void setDow(String dow) {
		this.dow = dow;
	}
	public void setT_start(String t_start) {
		this.t_start = t_start;
	}
	public void setT_end(String t_end) {
		this.t_end = t_end;
	}
	public void setGroups(String groups) {
		this.groups = groups;
	}
}
