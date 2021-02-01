import wx
import os
import serial.tools.list_ports
from lib.mcu_control import MCUControl

class MainWindow(wx.Frame):
    def __init__(self, parent, title):

        self.mcu = None

        self.dirname=''

        # A "-1" in the size parameter instructs wxWidgets to use the default size.
        # In this case, we select 200px width and the default height.
        wx.Frame.__init__(self, parent, title=title, size=(600, 300))

        self.timer = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.update, self.timer)
        self.timer.Start(milliseconds=200)

        self.control = wx.TextCtrl(self, style=wx.TE_MULTILINE)

        self.sizer2 = wx.BoxSizer(wx.HORIZONTAL)

        self.sizer3 = wx.BoxSizer(wx.VERTICAL)

        self.acc_register = wx.StaticText(self, 0, label="-")
        self.dat_register = wx.StaticText(self, 0, label="-")
        self.sizer3.AddSpacer(10)
        self.sizer3.Add(wx.StaticText(self, 0, label="ACC"), 0)
        self.sizer3.Add(self.acc_register, 0)
        self.sizer3.AddSpacer(5)
        self.sizer3.Add(wx.StaticLine(self, 0, style = wx.LI_HORIZONTAL), 0, wx.EXPAND)
        self.sizer3.AddSpacer(5)
        self.sizer3.Add(wx.StaticText(self, 0, label="DAT"), 0)
        self.sizer3.Add(self.dat_register, 0)
        self.sizer3.AddSpacer(5)
        self.sizer3.Add(wx.StaticLine(self, 0, style = wx.LI_HORIZONTAL), 0, wx.EXPAND)

        self.sizer2.Add(self.control, 1, wx.EXPAND)
        self.sizer2.AddSpacer(5)
        self.sizer2.Add(self.sizer3, 0, wx.EXPAND)
        self.sizer2.AddSpacer(5)

        self.CreateStatusBar() # A Statusbar in the bottom of the window

        # Setting up the menu.
        filemenu= wx.Menu()
        menuOpen = filemenu.Append(wx.ID_OPEN, "&Open"," Open a file to edit")
        menuAbout= filemenu.Append(wx.ID_ABOUT, "&About"," Information about this program")
        menuExit = filemenu.Append(wx.ID_EXIT,"E&xit"," Terminate the program")

        # Creating the menubar.
        menuBar = wx.MenuBar()
        menuBar.Append(filemenu, "&File")  # Adding the "filemenu" to the MenuBar
        self.SetMenuBar(menuBar)  # Adding the MenuBar to the Frame content.

        # Events.
        self.Bind(wx.EVT_MENU, self.OnOpen, menuOpen)
        self.Bind(wx.EVT_MENU, self.OnExit, menuExit)
        self.Bind(wx.EVT_MENU, self.OnAbout, menuAbout)

        self.sizer4 = wx.BoxSizer(wx.HORIZONTAL)
        self.buttons = []

        self.upload_button = wx.Button(self, -1, "Upload")
        self.upload_button.Bind(wx.EVT_BUTTON, self.OnUpload)

        self.serial_selector = self.getSerialChoiceWidget()

        self.mcu_selector = wx.Choice(self, -1, choices=["1", "2", "3"])

        self.sizer4.AddSpacer(5)
        self.sizer4.Add(wx.StaticText(self, 0, label="Serialport: "), -1)
        self.sizer4.Add(self.serial_selector, 1)
        self.sizer4.AddSpacer(5)
        self.sizer4.Add(wx.StaticText(self, 0, label="MCU ID: "), -1)
        self.sizer4.Add(self.mcu_selector, 1)
        self.sizer4.AddSpacer(5)
        self.sizer4.Add(self.upload_button, 1)
        self.sizer4.AddSpacer(5)

        # Use some sizers to see layout options
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.sizer.Add(self.sizer2, 1, wx.EXPAND)
        self.sizer.Add(self.sizer4, 0, wx.EXPAND)

        #Layout sizers
        self.SetSizer(self.sizer)
        self.SetAutoLayout(1)
        #self.sizer.Fit(self)
        self.Show()

    def update(self, e):
        if self.mcu:
            registers = self.mcu.GetStatus()
            if registers and len(registers) == 2:
                self.acc_register.SetLabel(str(registers[0]))
                self.dat_register.SetLabel(str(registers[1]))



    def OnAbout(self, e):
        # Create a message dialog box
        dlg = wx.MessageDialog(self, " An IDE for the MCxxxx series of processors", "", wx.OK)
        dlg.ShowModal() # Shows it
        dlg.Destroy() # finally destroy it when finished.

    def OnExit(self,e):
        self.Close(True)  # Close the frame.

    def OnOpen(self,e):
        """ Open a file"""
        dlg = wx.FileDialog(self, "Choose a file", self.dirname, "", "*.*", wx.FD_OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            self.filename = dlg.GetFilename()
            self.dirname = dlg.GetDirectory()
            f = open(os.path.join(self.dirname, self.filename), 'r')
            self.control.SetValue(f.read())
            f.close()
        dlg.Destroy()

    def getSerialChoiceWidget(self):
        ports = []
        default = 0
        for i, port in enumerate(serial.tools.list_ports.comports()):
            ports.append(port[0])
            if not default and "usbserial" in port[0] or "SLAB" in port[0]:
                default = i
        widget = wx.Choice(self, -1, choices=ports)
        widget.SetSelection(default)
        return widget

    def OnUpload(self,e):
        if self.mcu:
            del(self.mcu)

        self.timer.Stop()

        serial_port = self.serial_selector.GetString(self.serial_selector.GetSelection())

        self.mcu = MCUControl(serial_port)

        code = self.control.GetValue()
        mcu_id = self.mcu_selector.GetString(self.mcu_selector.GetSelection())
        upload_error = self.mcu.UploadCode(code, mcu_id)
        if upload_error is not None:
            print(upload_error)
        self.timer.Start()


app = wx.App(False)
frame = MainWindow(None, "MCxxxx IDE V0.2 alpha")
app.MainLoop()
