function autoRefreshDashboard() {
  var sheet = SpreadsheetApp.getActiveSpreadsheet();

  // Refresh "Dashboard" formulas
  sheet.getSheetByName("Dashboard").getRange("C1").setValue("Updated: " + new Date());

  // Refresh all charts on the "Dashboard" sheet
  var dashboardSheet = sheet.getSheetByName("Dashboard");
  var charts = dashboardSheet.getCharts();
  
  for (var i = 0; i < charts.length; i++) {
    dashboardSheet.updateChart(charts[i]);
  }
}