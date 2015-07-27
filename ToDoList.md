# Introduction #

Here we can list things that need to be done and who is working on them


# Details #

## Wizard section ##

  * Determine which style to use. (Currently ModernStyle seems to work fine in Win7 and Linux)
    * option B here would be to code for specific styles, or fix it so it works in both.
  * Add a loading animation/progress bar when waiting for facebook pages to load
    * This should be easy. There's a couple different signals emitted by QWebkit we can hook into. Basically show our "loading" widget until we get the signal that the inital page load is done, then swap it out for the WebView (m\_view) object
  * Rethink the facebook connect stuff. If we want to do chat we'll need to bypass the initial facebook conncet login screen, do out own user/pass dialog, then hit the normal login URL. I've done this with curl, it works and you get the login cookies. We then pull the channel info we need for chat out of the initial response, then hit the permissions\_request url.

---




## Main Application ##

  * Single threaded? Seems like the obvious choice and rely on the Qt event framework as use [QTimer](http://qt.nokia.com/doc/4.6/qtimer.html) for running API calls to check for new comments, etc. It also simplifies things in that we can use a single instance of [QNetworkAccessManager](http://qt.nokia.com/doc/4.6/qnetworkaccessmanager.html) (it's not threadsafe)

  * There's a chance that the "permanent" session\_key can be expired by facebook (apparently they don't have a dictionary). There's an API method to verify the session (Users.getLoggedInUser) - it should return the user's UID to crosscheck against the one we have stored in the UserInfo object.
    * Add check at application start
    * Send user to wizard if fails
      * There is an alternate text for the initial wizard page. If you set the third argument to the FBConnectionWizard constructor (bool firstTime) to 'false' it will display the alternate text explaining that facebook killed the session and they need to log in again

  * What to do next?
    * My thoughts on the design are to revolve everything around a notification icon that sits in the system tray (windows) / top-bar on linux/OSX
      * There's a chapter in the Qt programming book on safari. I'm about to read it
    * Decide on how to build API requests
      * **`[`DONE`]`** My thought there was a factory that returned various API call objects. Each would encapsulate the required arguments for each API method, provide setters for them, sanity checking, and then an execute() method. There would also be single QNEtworkAccessManager object (Factory would be singleton?) Thoughts?
      * Do XML parsing inside the `API::Method` derived classes. See [TheApiNamespace](TheApiNamespace.md) for my thoughts on this.
    * Start building widgets for displaying various components and allowing for input
      * The app will revolve around the system tray icon. We need to decide on the UI. What happens when you click on it? What's the context menu going to look like?
      * One thing for certain we'll need is a preferences widget for various options